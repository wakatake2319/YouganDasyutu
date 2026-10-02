#include "TitleScene.h"
#include "SceneManager.h"
#include "math.h"
#include <numbers>

void TitleScene::Finalize() {
	delete titleSceneSprite_; 
	delete titleSprite_;
	delete creditSceneSprite_;
	// ステージ画像スプライト削除
	for (int i = 0; i < kMaxStage; i++) {

		delete stageSprites_[i];
		stageSprites_[i] = nullptr;
	}
}
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
	// 決定音
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/kettei.wav");

	Audio::GetInstance()->SetVolume(AudioPlayHandle_, 0.2f);

	titleState_ = TitleState::Title;
	isSceneChanging_ = false;

	// ステージ画像
	stageTH_[0] = TextureManager::Load("StageSelect/stage_1.png");
	stageTH_[1] = TextureManager::Load("StageSelect/stage_2.png");
	stageTH_[2] = TextureManager::Load("StageSelect/stage_3.png");
	stageTH_[3] = TextureManager::Load("StageSelect/stage_4.png");
	stageTH_[4] = TextureManager::Load("StageSelect/stage_5.png");

	for (int i = 0; i < kMaxStage; i++) {

		stageSprites_[i] = Sprite::Create(stageTH_[i], {64.0f, 64.0f});
		stageSprites_[i]->SetSize({512.0f*1.2f, 192.0f*1.2f});
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

}
void TitleScene::Update() {
	if (isSliding_) {

		slideTimer_++;

		if (slideTimer_ >= kSlideDuration_) {

			slideTimer_ = kSlideDuration_;

			stageIndex_ = nextStage_;

			isSliding_ = false;
		}
	}


	counter_ += 1.0f / 60.0f;
	// カウンターが1秒を超えたら、スペースキーを押すとタイトルに戻るようにする
	if (counter_ > kTimeTitleMove) {

		switch (titleState_) {
		case TitleState::Title:
			// 通常のタイトル画面の更新
			// Cキーが押されたらクレジット画面に遷移
			if (Input::GetInstance()->TriggerKey(DIK_C) && titleState_ == TitleState::Title && !isSceneChanging_) {
				titleState_ = TitleState::Credit;
				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
			}

			if (Input::GetInstance()->TriggerKey(DIK_SPACE) && titleState_ == TitleState::Title && !isSceneChanging_) {

				// シーン切り替え中にする
				isSceneChanging_ = true;

				// ステージセレクトに遷移する
				titleState_ = TitleState::StageSelect;


				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
				//  音を止める
				//if (Audio::GetInstance()->IsPlaying(AudioPlayHandle_)) {
				//	Audio::GetInstance()->StopWave(AudioPlayHandle_);
				//}

				// ゲームシーンに遷移する
				//SceneManager::GetInstance()->ChangeScene("Game");
			}

			if (Input::GetInstance()->TriggerKey(DIK_T) && titleState_ == TitleState::Title && !isSceneChanging_) {
				// ゲーム説明に遷移する
				titleState_ = TitleState::GameDescription;
				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);

			}
			break;
		case TitleState::Credit:
			// クレジット画面の更新
			// Bキーが押されたらタイトル画面に遷移
			if (Input::GetInstance()->TriggerKey(DIK_B) && titleState_ == TitleState::Credit && !isSceneChanging_) {
				titleState_ = TitleState::Title;
				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
			}
			break;
		case TitleState::StageSelect:
			// ステージセレクト画面の更新
			inputCounter_ += 1.0f / 60.0f;
			isSceneChanging_ = false;

			if (inputCounter_ > kTimeTitleMove) {

				// Aキーが押されたらステージ1に遷移
				if (Input::GetInstance()->TriggerKey(DIK_A) && titleState_ == TitleState::StageSelect) {
					// 効果音を鳴らす
					Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);

					RightArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

					inputCounter_ = 0.0f;

					// ステージ番号を設定
					nextStage_ = stageIndex_ - 1;

					if (nextStage_ < 0) {
						nextStage_ = kMaxStage - 1;
					}

					slideDirection_ = -1;

					isSliding_ = true;
					slideTimer_ = 0.0f;
				} else {
					RightArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});
				}

				// Dキーが押されたらステージ1に遷移
				if (Input::GetInstance()->TriggerKey(DIK_D) && titleState_ == TitleState::StageSelect) {
					// 効果音を鳴らす
					Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);

					LeftArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

					inputCounter_ = 0.0f;

					// ステージ番号を設定
					nextStage_ = stageIndex_ + 1;

					if (nextStage_ >= kMaxStage) {
						nextStage_ = 0;
					}

					slideDirection_ = +1;

					isSliding_ = true;
					slideTimer_ = 0.0f;
				} else {
					LeftArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});
				}

				if (Input::GetInstance()->TriggerKey(DIK_SPACE) && titleState_ == TitleState::StageSelect) {
					// 効果音を鳴らす
					Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);

					inputCounter_ = 0.0f;


					// SceneManagerにステージ番号を渡す
					SceneManager::GetInstance()->SetStage(stageIndex_);

					//  音を止める
					if (Audio::GetInstance()->IsPlaying(AudioPlayHandle_)) {
						Audio::GetInstance()->StopWave(AudioPlayHandle_);
					}

					// ゲームシーンに遷移する
					SceneManager::GetInstance()->ChangeScene("Game");
				}
			}
			// Bキーが押されたらタイトル画面に遷移
			if (Input::GetInstance()->TriggerKey(DIK_B) && titleState_ == TitleState::StageSelect && !isSceneChanging_) {
				titleState_ = TitleState::Title;
				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
			}
			break;
		case TitleState::GameDescription:
			// ゲーム説明画面の更新
			if (Input::GetInstance()->TriggerKey(DIK_B) && titleState_ == TitleState::GameDescription && !isSceneChanging_) {
				titleState_ = TitleState::Title;
				// 効果音を鳴らす
				Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
			}
			break;
		}
	}
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	switch (titleState_) {
	case TitleState::Title:
		titleSceneSprite_->Draw();
		titleSprite_->Draw();
		break;
	case TitleState::Credit:
		titleSceneSprite_->Draw();
		creditSceneSprite_->Draw();
		break;
	case TitleState::StageSelect:
		titleSceneSprite_->Draw();
		LeftArrowSprite_->Draw();
		RightArrowSprite_->Draw();
		selectStageLogoSprite_->Draw();
		// ステージセレクトのUIを描画する
		if (isSliding_) {

			float t = slideTimer_ / kSlideDuration_;

			float currentX = 640.0f - (t * 1280.0f * slideDirection_);

			float nextX = 640.0f + ((1.0f - t) * 1280.0f * slideDirection_);

			// 現在ステージ
			stageSprites_[stageIndex_]->SetPosition({currentX, 390.0f});
			stageSprites_[stageIndex_]->Draw();
			//stageLogoSprite_[stageIndex_]->SetPosition({currentX, 130.0f});
			//stageLogoSprite_[stageIndex_]->Draw();

			// 次ステージ
			stageSprites_[nextStage_]->SetPosition({nextX, 390.0f});
			stageSprites_[nextStage_]->Draw();
			//stageLogoSprite_[nextStage_]->SetPosition({nextX, 130.0f});
			//stageLogoSprite_[nextStage_]->Draw();

		} else {

			// 通常時は中央固定
			stageSprites_[stageIndex_]->SetPosition({640.0f, 390.0f});
			stageSprites_[stageIndex_]->Draw();
			//stageLogoSprite_[stageIndex_]->SetPosition({640.0f, 130.0f});
			//stageLogoSprite_[stageIndex_]->Draw();
		}
		break;
	case TitleState::GameDescription:
		titleSceneSprite_->Draw();
		gameDescriptionSprite_->Draw();
		break;
	}

	Sprite::PostDraw();
}