#pragma once
#include "ICoinState.h"

class CoinGetState : public ICoinState {
public:
	// 更新
	void Update(Coin* coin) override;
};