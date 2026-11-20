#include "Lava.h"
#include "Math.h"
#include "Player.h"

void Lava::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// 速度設定
	velocity_ = {0, -kWalkSpeed, 0};

	WorldTransformUpdate(worldTransform_);
}
void Lava::Update() {

	// 移動
	worldTransform_.translation_ += velocity_;

	WorldTransformUpdate(worldTransform_);
}
void Lava::Draw() {
	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Lava::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Lava::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Lava::OnCollision() {

	// デスフラグを立てる
	isDeath_ = true;
}