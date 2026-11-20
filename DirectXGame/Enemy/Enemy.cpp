#define NOMINMAX
#include "Enemy.h"
//#include "Math.h"
#include "Player.h"
#include "MapChipField.h"
#include <numbers>

void Enemy::Initialize(Model* model, Camera* camera, MapChipField* mapchipField, const Vector3& position) {
	model_ = model;
	camera_ = camera;
	mapchipField_ = mapchipField;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	baseRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
	walkAnimRotationY_ = 0.0f;
	worldTransform_.rotation_.y = baseRotationY_;
	// 速度設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}
void Enemy::Update() {

	// =========================
	// Behavior遷移の実装
	// =========================
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_;

		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		// 通常行動
		case Behavior::kDeath:
		default:
			counter_ = 0;
			break;
		}
		// 振る舞いリクエストを初期化
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kRoot: {
	default:
		BehaviorRootUpdate();
	break;
	}

		// デス演出
	case Behavior::kDeath: {

		// デス演出のカウンターを進める
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		// ワールド行列更新
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isDeath_ = true;
		}

		break;
	}
	}
}
void Enemy::Draw() {
	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) {

	// デスフラグを立てる
	isDeath_ = true;
	if (behavior_ == Behavior::kDeath) {
		// 敵がやられているなら何もしない
		return;
	}
	// プレイヤーが攻撃中なら敵が死ぬ
	if (player->IsAttack()) {
	
		// 衝突無効化
		behaviorRequest_ = Behavior::kDeath;
	}
}

void Enemy::OnHitByAttack() {

	if (behavior_ == Behavior::kDeath) {
		return;
	}

	behaviorRequest_ = Behavior::kDeath;
}

void Enemy::BehaviorRootUpdate() {
	if (walkTimer_ >= 500.0f || !IsGroundAhead()) {
		walkTimer_ = 0.0f;
		velocity_ *= -1.0f;

		// 反転後の向きに合わせて回転を設定する
		if (velocity_.x > 0.0f) {
			// 右向き
			baseRotationY_ = std::numbers::pi_v<float> / 2.0f;
		} else {
			// 左向き
			baseRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
		}
	}

	// 移動
	//worldTransform_.translation_ += velocity_;

	// =========================
	// 重力
	// =========================
	if (!onGround_) {
		velocity_.y -= kGravityAcceleration / 60.0f;

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// =========================
	// 衝突情報
	// =========================
	CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity_;

	// マップ衝突判定
	MapCollision(collisionMapInfo);

	// 移動
	worldTransform_.translation_ += collisionMapInfo.move;

	// 接地判定
	isGround(collisionMapInfo);

	// 壁に当たったら反転
	if (collisionMapInfo.wallContact) {
		velocity_.x *= -1.0f;

		if (velocity_.x > 0.0f) {
			baseRotationY_ = std::numbers::pi_v<float> / 2.0f;
		} else {
			baseRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
		}
	}

	// 歩行アニメーション
	const float kWalkSwing = ToRadians(5.0f);

	walkAnimRotationY_ = sinf(walkTimer_ * 0.1f) * kWalkSwing;

	worldTransform_.rotation_.y = baseRotationY_ + walkAnimRotationY_;

	// タイマー加算
	walkTimer_++;

	WorldTransformUpdate(worldTransform_);
}

void Enemy::MapCollision(CollisionMapInfo& info) {
	MapCollisionDown(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);
}
void Enemy::MapCollisionDown(CollisionMapInfo& info) {

	// 下降していない場合は処理しない
	if (info.move.y >= 0.0f) {
		return;
	}

	// 移動後の角の座標
	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 左下
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	MapChipType mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	MapChipType mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// 移動先のマップチップ
		indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex) {

			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));

			info.landing = true;
		}
	}
}
void Enemy::MapCollisionRight(CollisionMapInfo& info) {

	if (info.move.x <= 0.0f) {
		return;
	}

	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 右上
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	MapChipType mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	MapChipType mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {

			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));

			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));

			info.wallContact = true;
		}
	}
}
void Enemy::MapCollisionLeft(CollisionMapInfo& info) {

	if (info.move.x >= 0.0f) {
		return;
	}

	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	MapChipType mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	MapChipType mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 左下
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {

			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));

			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

			info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));

			info.wallContact = true;
		}
	}
}
void Enemy::isGround(const CollisionMapInfo& info) {

	if (onGround_) {

		// 上方向に移動している場合は空中状態
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			std::array<Vector3, knumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			bool hit = false;

			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));

			MapChipType mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));

			mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			if (!hit) {
				onGround_ = false;
			}
		}

	} else {

		if (info.landing) {
			onGround_ = true;
			velocity_.y = 0.0f;
		}
	}
}
Vector3 Enemy::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  // kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

bool Enemy::IsGroundAhead() {
	// 進行方向に地面があるかどうかの判定
	Vector3 aheadPosition = worldTransform_.translation_ + Vector3(velocity_.x > 0 ? kWidth / 2.0f : -kWidth / 2.0f, -kHeight / 2.0f - kGroundSearchHeight, 0);
	MapChipField::IndexSet indexSet = mapchipField_->GetMapChipIndexSetByPosition(aheadPosition);
	MapChipType mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	return mapChipType == MapChipType::kBlock;
}

void Enemy::SetMoveVector(const Vector3& moveVector) { velocity_ = moveVector; }