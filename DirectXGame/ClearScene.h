#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"

using namespace KamataEngine;

class ClearScene : public SceneBase {
public:
	ClearScene(KamataEngine::Input* input) : SceneBase(input) {}

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeClearMove = 0.5f;

	// ビュープロジェクション
	Camera camera_;

	// クリアシーンのテクスチャ
	uint32_t clearSceneTH_ = 0;
	Sprite* clearSceneSprite_ = nullptr;

	float counter_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;
	// シーン切り替え中かどうか
	bool isSceneChanging_ = false;

	// フェード
	//Fade* fade_ = nullptr;


	// BGM
	uint32_t BGMHandle_ = 0;
	uint32_t AudioPlayHandle_ = 0;

	// SE
	uint32_t SEHandle_ = 0;
	uint32_t SEAudioHandle_ = 0;
};
