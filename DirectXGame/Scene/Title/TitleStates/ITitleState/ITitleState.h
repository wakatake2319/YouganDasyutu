#pragma once

class TitleScene;

class ITitleState {
public:
	virtual ~ITitleState() = default;

	// 更新
	virtual void Update(TitleScene* titleScene) = 0;

	// 描画
	virtual void Draw(TitleScene* titleScene) = 0;
};