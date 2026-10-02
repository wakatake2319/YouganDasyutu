#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"

using namespace KamataEngine;

class TitleScene : public SceneBase {
public:
	TitleScene(Input* input) : SceneBase(input) {}

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
	static inline const float kTimeTitleMove = 0.5f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	// タイトルシーンのテクスチャ
	uint32_t titleSceneTH_ = 0;
	Sprite* titleSceneSprite_ = nullptr;

	// タイトルの文字のテクスチャ
	uint32_t titleTH_ = 0;
	Sprite* titleSprite_ = nullptr;

	// クレジットシーンのテクスチャ
	uint32_t creditSceneTH_ = 0;
	Sprite* creditSceneSprite_ = nullptr;

	// ステージの横にだす右矢印
	uint32_t RightArrowTH_ = 0;
	// ステージの横にだす右矢印スプライト
	Sprite* RightArrowSprite_ = nullptr;

	// ステージの横にだす左矢印
	uint32_t LeftArrowTH_ = 0;
	// ステージの横にだす左矢印スプライト
	Sprite* LeftArrowSprite_ = nullptr;

	// セレクトステージのロゴ
	uint32_t selectStageLogoTH_ = 0;
	// セレクトステージのロゴスプライト
	Sprite* selectStageLogoSprite_ = nullptr;

	// ゲームの説明
	uint32_t gameDescriptionTH_ = 0;
	Sprite* gameDescriptionSprite_ = nullptr;

	float counter_ = 0.0f;

	// ステージ選択の時に連続で同じキーを押さないようにするためのカウンター
	float inputCounter_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;

	// シーン切り替え中かどうか
	bool isSceneChanging_ = false;

	// 選択中ステージ
	int stageIndex_ = 0;
	static const int kMaxStage = 5;
	// 次に表示するステージ
	int nextStage_ = 0;

	// スライド中か
	bool isSliding_ = false;

	// スライドタイマー
	float slideTimer_ = 0.0f;

	// スライド時間
	const float kSlideDuration_ = 20.0f;

	// スライド方向
	int slideDirection_ = 1; // 右:1 左:-1

	// ステージ画像
	uint32_t stageTH_[kMaxStage];
	// ステージ画像スプライト
	KamataEngine::Sprite* stageSprites_[kMaxStage];

	// フェード
	//Fade* fade_ = nullptr;

	// 現在のフェード
	//Phase phase_ = Phase::kFadeIn;

	// タイトルの状態
	enum class TitleState {
		Title,  // 通常
		Credit, // クレジット
		StageSelect, // ステージセレクト
		GameDescription, // ゲーム説明
	};
	TitleState titleState_ = TitleState::Title;

	// BGM
	uint32_t BGMHandle_ = 0;
	uint32_t AudioPlayHandle_ = 0;

	// SE
	uint32_t SEHandle_ = 0;
	uint32_t SEAudioHandle_ = 0;
};