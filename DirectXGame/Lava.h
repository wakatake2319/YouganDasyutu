#pragma once
#include "KamataEngine.h"
#include "Math.h"

class Player;

class Lava {

public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABB取得関数
	AABB GetAABB();

	// 衝突判定
	void OnCollision();

	// 衝突時コールバック
	void OnHitByAttack();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.025f;
	// 速度
	Vector3 velocity_ = {};

	// 当たり判定の大きさ
	static inline const float kWidth = 75.0f;
	static inline const float kHeight = 1.0f;

	// デスフラグ
	bool isDeath_ = false;
};
