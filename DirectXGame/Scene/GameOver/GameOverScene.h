#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"

using namespace KamataEngine;

class GameOverScene : public SceneBase {
public:
	GameOverScene(KamataEngine::Input* input) : SceneBase(input) {}

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了
	void Finalize() override;

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeGameOverMove = 0.5f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformGameOver_;
	WorldTransform worldTransformPlayer_;

	// ゲームオーバーシーンのテクスチャ
	uint32_t gameoverSceneTH_ = 0;
	Sprite* gameoverSceneSprite_ = nullptr;

	float counter_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;
	// シーン切り替え中かどうか
	bool isSceneChanging_ = false;


	// BGM
	uint32_t BGMHandle_ = 0;
	uint32_t AudioPlayHandle_ = 0;

	// SE
	uint32_t SEHandle_ = 0;
	uint32_t SEAudioHandle_ = 0;
};
