#pragma once
#include "ICoinState.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <memory>

using namespace KamataEngine;

class Player;
class CoinRootState;
class CoinGetState;

class Coin {

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
	void OnCollision(const Player* player);

	// 取ったかどうか
	bool IsGet() const { return isGet_; }

	// 衝突無効化
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	// 終了判定
	bool IsFinished() { return isFinished_; }

	// 状態変更
	void ChangeState(std::unique_ptr<ICoinState> newState);

private:
	friend class CoinRootState;
	friend class CoinGetState;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.02f;

	// ===================
	// アニメーション
	// ===================

	// 最初の角度
	static inline const float kRotateMotionAngleStart = 0.0f;

	// 最後の角度
	static inline const float kRotateMotionAngleEnd = 30.0f;

	// アニメーションの周期となる時間
	static inline const float kRotateMotionTime = 1.0f;

	// 経過時間
	float rotateTimer_ = 0.0f;

	// 当たり判定の大きさ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 取るフラグ
	bool isGet_ = false;

	// 取得演出の時間
	static inline const float kDefeatedTime = 0.6f;

	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;

	float counter_ = 0.0f;

	// 衝突判定無効化
	bool isCollisionDisabled_ = false;

	// 基準位置
	Vector3 basePosition_;

	// 終了フラグ
	bool isFinished_ = false;

	// 現在の状態
	std::unique_ptr<ICoinState> currentState_;

	// SE
	uint32_t SEHandle_ = 0;
	uint32_t SEAudioHandle_ = 0;
};