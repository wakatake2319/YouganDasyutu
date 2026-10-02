#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

// 前方宣言
class MapChipField;
class Goal;
class Lava;
class Coin;

class Player {
public:
	// 左右の振り向き
	enum class LRDirection {
		kRight,
		kLeft,
	};

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

	// ビヘイビア
	enum class Behavior {
		// 未定義
		kUnknown = -1,
		// 通常状態
		kRoot,
	};

	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 速度加算
	const Vector3& GetVelocity() const { return velocity_; }

	// マップチップフィールド
	void SetMapChipField(MapChipField* mapchipField) { mapchipField_ = mapchipField; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABB取得関数
	AABB GetAABB();

#pragma region ゴールとの衝突判定関係
	// ゴールと当たった時にゴールゲットフラグを取る
	void OnCollisionGoal(const Goal* goal);

	// ゴールのゲットフラグのsetter
	void SetIsGoalGet(bool isGoalGet) { isGoalGet_ = isGoalGet; }

	// ゴールのゲットフラグのgetter
	bool IsGoalGet() const { return isGoalGet_; }
#pragma endregion

#pragma region コインとの衝突判定関係
	// コインと当たった時にコインゲットフラグを取る
	void OnCollisionCoin(const Coin* coin);

	// コインのゲットフラグのsetter
	void SetIsCoinGet(bool isCoinGet) { isCoinGet_ = isCoinGet; }

	// コインのゲットフラグのgetter
	bool IsCoinGet() const { return isCoinGet_; }
#pragma endregion

#pragma region 溶岩との衝突判定関係
	// 溶岩と当たった時にデスフラグを取る
	void OnCollisionLava(const Lava* lava);

	// デスフラグのgetter
	bool IsDeath() const { return isDeath_; }
#pragma endregion

	// 通常行動更新
	void BehaviorRootUpdate();

	// 通常行動初期化
	void BehaviorRootInitialize();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {};

	// ================================
	// 移動
	// ================================
	// 地上慣性移動
	static inline const float kAcceleration = 0.006f;
	// 地上減速
	static inline const float kAtteleration = 0.1f;
	// 空中慣性移動
	static inline const float kAccelerationOnSky = 0.003f;
	// 空中減速
	static inline const float kAttelerationOnsky = 0.1f;
	// 最大速度
	static inline const float kLimitRunSpeed = 0.25f;
	// 空中ジャンプができるか
	bool isAirJump = false;
	// 空中ジャンプできるまでのラグ
	static inline const int kAirJumpLag = 3;
	// 空中ジャンプまでのラグタイマー
	int airJumpLagTimer = 0;

	// ===============================
	// 振り向き
	// ===============================
	// 角度補強
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.3f;
	// 左右の振り向き
	LRDirection lrDirection_ = LRDirection::kRight;

	// ===============================
	// 着地
	// ===============================
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.25f;
	// 地上ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;
	// 空中ジャンプ初速(上方向)
	static inline const float kAirJumpAcceleration = 10.0f;

	// ================================
	// プレイヤーの当たり判定
	// ================================
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.04f;

	// ======================
	// 移動処理
	// ======================
	void InputMove();
	// マップチップとの当たり判定情報
	struct CollisionMapInfo {

		// 天井衝突フラグ
		bool ceilingCollisionFlag = false;

		// 着地フラグ
		bool landing = false;

		// 壁接触フラグ
		bool wallContact = false;

		// 移動量
		Vector3 move;
	};

	// 移動量を加味して衝突判定する
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionDown(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	// 指定した過度の座標計算
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 当たり判定
	MapChipField* mapchipField_ = nullptr;

	// 接地状態の切り替え
	void isGround(const CollisionMapInfo& info);

	// 壁接触状態の切り替え
	void isWall(const CollisionMapInfo& info);

	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	// 壁接触時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	// デスフラグ
	bool isDeath_ = false;

	// ゴールのゲットフラグ
	bool isGoalGet_ = false;

	// コインのゲットフラグ
	bool isCoinGet_ = false;

	// ================================
	// Behaviorの定義
	// ================================
	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いのリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;
	// カメラ
	Camera* camera_;
};