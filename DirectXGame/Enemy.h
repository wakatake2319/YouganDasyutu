#pragma once
#include "KamataEngine.h"
#include "Math.h"
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

	// マップチップフィールド
	//void SetMapChipField(MapChipField* mapchipField) { mapchipField_ = mapchipField; }

	void BehaviorRootUpdate();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

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

	//	// マップチップとの当たり判定情報
	//struct CollisionMapInfo {
	//
	//	// 天井衝突フラグ
	//	bool ceilingCollisionFlag = false;
	//
	//	// 着地フラグ
	//	bool landing = false;
	//
	//	// 壁接触フラグ
	//	bool wallContact = false;
	//
	//	// 移動量
	//	Vector3 move;
	//};
	//
	//// 移動量を加味して衝突判定する
	//void MapCollision(CollisionMapInfo& info);
	//void MapCollisionUp(CollisionMapInfo& info);
	//void MapCollisionDown(CollisionMapInfo& info);
	//void MapCollisionRight(CollisionMapInfo& info);
	//void MapCollisionLeft(CollisionMapInfo& info);
	//
	//// 指定した過度の座標計算
	//Vector3 CornerPosition(const Vector3& center, Corner corner);
	//
	//// 当たり判定
	//MapChipField* mapchipField_ = nullptr;
	//
	//// 接地状態の切り替え
	//void isGround(const CollisionMapInfo& info);
	//
	//// 壁接触状態の切り替え
	//void isWall(const CollisionMapInfo& info);
	//
	//// 着地時の速度減衰率
	//static inline const float kAttenuationLanding = 0.1f;
	//// 微小な数値
	//static inline const float kGroundSearchHeight = 0.06f;
	//// 壁接触時の速度減衰率
	//static inline const float kAttenuationWall = 0.2f;
	//// 接地状態フラグ
	//bool onGround_ = true;
	//// ================================
	//// 敵の当たり判定
	//// ================================
	//
	//static inline const float kBlank = 0.04f;
};