#pragma once
#include "ITitleState.h"

class TitleMainState : public ITitleState {
public:
	void Update(TitleScene* titleScene) override;
	void Draw(TitleScene* titleScene) override;
};