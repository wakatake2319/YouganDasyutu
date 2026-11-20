#include "TitleScene.h"
#include "SceneManager.h"
#include "ITitleState.h"
#include "TitleMainState.h"
#include <math.h>
#include <numbers>

// 終了
void TitleScene::Finalize() {

	delete titleSceneSprite_;
	titleSceneSprite_ = nullptr;

	delete titleSprite_;
	titleSprite_ = nullptr;

	delete creditSceneSprite_;
	creditSceneSprite_ = nullptr;

	delete gameDescriptionSprite_;
	gameDescriptionSprite_ = nullptr;

	delete RightArrowSprite_;
	RightArrowSprite_ = nullptr;

	delete LeftArrowSprite_;
	LeftArrowSprite_ = nullptr;

	delete selectStageLogoSprite_;
	selectStageLogoSprite_ = nullptr;

	// ステージ画像スプライト削除
	for (int i = 0; i < kMaxStage; i++) {
		delete stageSprites_[i];
		stageSprites_[i] = nullptr;
	}

	currentState_.reset();
}

// 初期化
void TitleScene::Initialize() {

	titleSceneTH_ = TextureManager::Load("SceneTexture/titleScene.png");
	titleSceneSprite_ = Sprite::Create(titleSceneTH_, Vector2(0.0f, 0.0f));
	titleSceneSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// タイトルの文字の初期化
	titleTH_ = TextureManager::Load("SceneTexture/title_text.png");
	titleSprite_ = Sprite::Create(titleTH_, Vector2(0.0f, 0.0f));
	titleSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// クレジットシーンの初期化
	creditSceneTH_ = TextureManager::Load("SceneTexture/credit.png");
	creditSceneSprite_ = Sprite::Create(creditSceneTH_, Vector2(0.0f, 0.0f));
	creditSceneSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// ゲーム説明の初期化
	gameDescriptionTH_ = TextureManager::Load("SceneTexture/game_description.png");
	gameDescriptionSprite_ = Sprite::Create(gameDescriptionTH_, Vector2(0.0f, 0.0f));
	gameDescriptionSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// BGM
	BGMHandle_ = Audio::GetInstance()->LoadWave("audio/BGM/BGM2.wav");
	AudioPlayHandle_ = Audio::GetInstance()->PlayWave(BGMHandle_, true, 0.5f);

	// SEの読み込み
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/kettei.wav");

	Audio::GetInstance()->SetVolume(AudioPlayHandle_, 0.2f);

	isSceneChanging_ = false;
	finished_ = false;

	counter_ = 0.0f;
	inputCounter_ = 0.0f;

	stageIndex_ = 0;
	nextStage_ = 0;

	isSliding_ = false;
	slideTimer_ = 0.0f;
	slideDirection_ = 1;

	// ステージ画像
	stageTH_[0] = TextureManager::Load("StageSelect/stage_1.png");
	stageTH_[1] = TextureManager::Load("StageSelect/stage_2.png");
	stageTH_[2] = TextureManager::Load("StageSelect/stage_3.png");
	stageTH_[3] = TextureManager::Load("StageSelect/stage_4.png");
	stageTH_[4] = TextureManager::Load("StageSelect/stage_5.png");

	for (int i = 0; i < kMaxStage; i++) {

		stageSprites_[i] = Sprite::Create(stageTH_[i], {64.0f, 64.0f});
		stageSprites_[i]->SetSize({512.0f * 1.2f, 192.0f * 1.2f});
		stageSprites_[i]->SetAnchorPoint({0.5f, 0.5f});
	}

	// 右矢印
	RightArrowTH_ = TextureManager::Load("StageSelect/A.png");
	RightArrowSprite_ = Sprite::Create(RightArrowTH_, {192.0f, 390.0f});
	RightArrowSprite_->SetSize({128.0f, 128.0f});
	RightArrowSprite_->SetAnchorPoint({0.5f, 0.5f});
	RightArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});

	// 左矢印
	LeftArrowTH_ = TextureManager::Load("StageSelect/D.png");
	LeftArrowSprite_ = Sprite::Create(LeftArrowTH_, {1280.0f - 192.0f, 390.0f});
	LeftArrowSprite_->SetSize({128.0f, 128.0f});
	LeftArrowSprite_->SetAnchorPoint({0.5f, 0.5f});
	LeftArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});

	// セレクトステージのロゴ
	selectStageLogoTH_ = TextureManager::Load("StageSelect/StageSelect_logo.png");
	selectStageLogoSprite_ = Sprite::Create(selectStageLogoTH_, Vector2(0.0f, 0.0f));
	selectStageLogoSprite_->SetSize({1280.0f, 720.0f});

	// タイトル状態にする
	currentState_ = std::make_unique<TitleMainState>();
}

// 更新
void TitleScene::Update() {

	// スライド中の更新
	if (isSliding_) {

		slideTimer_++;

		if (slideTimer_ >= kSlideDuration_) {

			slideTimer_ = kSlideDuration_;

			stageIndex_ = nextStage_;

			isSliding_ = false;
		}
	}

	// 現在の状態に更新を委譲する
	if (currentState_) {
		currentState_->Update(this);
	}
}

// 描画
void TitleScene::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	// 現在の状態に描画を委譲する
	if (currentState_) {
		currentState_->Draw(this);
	}

	Sprite::PostDraw();
}

// 状態変更
void TitleScene::ChangeState(std::unique_ptr<ITitleState> newState) { currentState_ = std::move(newState); }