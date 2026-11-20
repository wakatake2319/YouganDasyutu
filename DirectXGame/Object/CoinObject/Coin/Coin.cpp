#include "Coin.h"
#include "Math.h"
#include "Player.h"
#include "CoinGetState.h"
#include "CoinRootState.h"
#include <numbers>

// 初期化
void Coin::Initialize(Model* model, Camera* camera, const Vector3& position) {

	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	basePosition_ = position;

	worldTransform_.scale_ = {0.5f, 0.5f, 1.0f};

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.0f / 2.0f;

	rotateTimer_ = 0.0f;

	// フラグ初期化
	isGet_ = false;
	isCollisionDisabled_ = false;
	isFinished_ = false;

	counter_ = 0.0f;

	// 初期状態を設定
	currentState_ = std::make_unique<CoinRootState>();

	// SEの読み込み
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/coin.wav");
}

// 更新
void Coin::Update() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// 現在の状態に更新を委譲
	if (currentState_) {
		currentState_->Update(this);
	}
}

// 状態変更
void Coin::ChangeState(std::unique_ptr<ICoinState> newState) { currentState_ = std::move(newState); }

// 描画
void Coin::Draw() {

	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

// ワールド座標を取得
Vector3 Coin::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// AABB取得
AABB Coin::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};

	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突判定
void Coin::OnCollision(const Player* player) {

	// すでに取得演出中、または取得済みなら何もしない
	if (isCollisionDisabled_) {
		return;
	}

	// 衝突判定を無効化
	isCollisionDisabled_ = true;

	// 取得演出のカウンターを初期化
	counter_ = 0.0f;

	// 取得演出の状態に変更
	ChangeState(std::make_unique<CoinGetState>());

	// 効果音を鳴らす
	Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);

	// プレイヤーにコイン取得を通知
	const_cast<Player*>(player)->IsCoinGet();
}