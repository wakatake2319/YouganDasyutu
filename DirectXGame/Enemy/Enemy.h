#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "CollisionMapInfo.h"

using namespace KamataEngine;

class Player;
class MapChipField;

class Enemy {

public:

	// 角
	enum Corner {
		// 右下
		kRightBottom,
		// 左下
		kLeftBottom,
		// 右上
		kRightTop,
		// 左上
		kLeftTop,

		// 要素数
		knumCorner
	};
	// 初期化
	void Initialize(Model* model, Camera* camera, MapChipField* mapchipField, const Vector3& position);

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

	// デスしたかどうか
	bool isDeath() const { return isDeath_; }

	// 衝突無効化
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	// ビヘイビア
	enum class Behavior {
		// 未定義
		kUnknown = -1,
		// 通常状態/歩行
		kRoot,
		// デス演出
		kDeath,
	};

	// 衝突時コールバック
	void OnHitByAttack();

	void BehaviorRootUpdate();

	// マップチップとの当たり判定
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionDown(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	// 接地判定
	void isGround(const CollisionMapInfo& info);

	// 角の座標計算
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 進行方向に地面があるかどうかのフラグ
	bool IsGroundAhead();

	// 移動方向の設定
	void SetMoveVector(const Vector3& moveVector);

	// 移動方向の取得
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// マップチップフィールド
	MapChipField* mapchipField_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.01f;
	// 速度
	Vector3 velocity_ = {};

	// ===================
	// アニメーション
	// ===================
	// 最初の角度
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 30.0f;
	// アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.0f;
	// 経過時間
	float walkTimer_ = 0.0f;

	float baseRotationY_ = 0.0f;     // 向き
	float walkAnimRotationY_ = 0.0f; // 歩行アニメーション

	// 当たり判定の大きさ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// デスフラグ
	bool isDeath_ = false;

	Behavior behavior_ = Behavior::kRoot;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	// 衝突判定無効化
	bool isCollisionDisabled_ = false;

	//// 接地状態フラグ
	bool onGround_ = true;

	// ================================
	// 重力・接地
	// ================================
	// 重力加速度
	static inline const float kGravityAcceleration = 9.8f;
	// 落下速度の上限
	static inline const float kLimitFallSpeed = 30.0f;
	//// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float kBlank = 0.04f;
};