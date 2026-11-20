#include "CoinRootState.h"
#include "Coin.h"
#include "Math.h"

void CoinRootState::Update(Coin* coin) {

	// タイマー加算
	coin->rotateTimer_ += 1.0f / 60.0f;

	// 回転
	coin->worldTransform_.rotation_.y += 0.02f;

	// ワールド行列更新
	WorldTransformUpdate(coin->worldTransform_);
}