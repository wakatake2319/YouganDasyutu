#pragma once
#include "ITitleState.h"

class StageSelectState : public ITitleState {
public:
	void Update(TitleScene* titleScene) override;
	void Draw(TitleScene* titleScene) override;
};