#include "Coin.h"
#include "Math.h"
#include "Player.h"
#include <numbers>

void Coin::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	basePosition_ = position; // 基準位置を保存
	worldTransform_.scale_ = {0.5f, 0.5f, 1.0f};

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.0f / 2.0f;

	rotateTimer_ = 0.0f;
	// SEの読み込み
	// 決定音
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/coin.wav");
}
void Coin::Update() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}

	// =========================
	// Behavior遷移の実装
	// =========================
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_;
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		// 通常行動
		case Behavior::kGet:
		default:
			counter_ = 0;
			break;
		}
		// 振る舞いリクエストを初期化
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kRoot:

		// タイマー加算
		rotateTimer_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.02f;

		// 回転アニメーション
		WorldTransformUpdate(worldTransform_);
		break;

		// 取る演出
	case Behavior::kGet:

		// 取る演出のカウンターを進める
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.2f;
		// 上に移動させる
		worldTransform_.translation_.y += 0.02f;

		// ワールド行列更新
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isGet_ = true;
			counter_ = kDefeatedTime;
			// 終了扱いにする
			isFinished_ = true;
		}

		break;
	}
}
void Coin::Draw() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Coin::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Coin::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Coin::OnCollision(const Player* player) {
	if (behavior_ == Behavior::kGet)
		return;
	behaviorRequest_ = Behavior::kGet;
	isCollisionDisabled_ = true;
	// 効果音を鳴らす
	Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
	// プレイヤーに鍵を渡す
	const_cast<Player*>(player)->IsCoinGet();
}