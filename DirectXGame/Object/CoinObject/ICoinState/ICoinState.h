#pragma once

class Coin;

class ICoinState {
public:
	virtual ~ICoinState() = default;

	// 更新
	virtual void Update(Coin* coin) = 0;
};