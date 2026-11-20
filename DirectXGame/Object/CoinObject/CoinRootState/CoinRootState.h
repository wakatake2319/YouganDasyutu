#pragma once
#include "ICoinState.h"

class CoinRootState : public ICoinState {
public:
	void Update(Coin* coin) override;
};