#include "wall.h"
#include "Math.h"
#include <numbers>

// 初期化
void wall::Initialize(Model* model, Camera* camera, const Vector3& position) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;

	worldTransform_.translation_ = position;

	// 反対を向ける
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
}

// 更新
void wall::Update() { WorldTransformUpdate(worldTransform_); }

// 描画
void wall::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
