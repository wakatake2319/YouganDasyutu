#include "Coin.h"
#include "CoinGetState.h"
#include "Math.h"

void CoinGetState::Update(Coin* coin) {

	// 取得演出のカウンターを進める
	coin->counter_ += 1.0f / 60.0f;

	// 回転
	coin->worldTransform_.rotation_.y += 0.2f;

	// 上に移動させる
	coin->worldTransform_.translation_.y += 0.02f;

	// ワールド行列更新
	WorldTransformUpdate(coin->worldTransform_);

	// 演出終了判定
	if (coin->counter_ >= Coin::kDefeatedTime) {

		coin->isGet_ = true;

		coin->counter_ = Coin::kDefeatedTime;

		// 終了扱いにする
		coin->isFinished_ = true;
	}
}