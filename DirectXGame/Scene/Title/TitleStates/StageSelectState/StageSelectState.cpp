#include "StageSelectState.h"
#include "TitleScene.h"
#include "TitleMainState.h"

#include "KamataEngine.h"
#include "SceneManager.h"

using namespace KamataEngine;

// 更新
void StageSelectState::Update(TitleScene* titleScene) {

	titleScene->counter_ += 1.0f / 60.0f;

	if (titleScene->counter_ <= TitleScene::kTimeTitleMove) {
		return;
	}

	titleScene->inputCounter_ += 1.0f / 60.0f;

	titleScene->isSceneChanging_ = false;

	if (titleScene->inputCounter_ > TitleScene::kTimeTitleMove) {

		// Aキーで前のステージ
		if (Input::GetInstance()->TriggerKey(DIK_A)) {

			Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

			titleScene->RightArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

			titleScene->inputCounter_ = 0.0f;

			titleScene->nextStage_ = titleScene->stageIndex_ - 1;

			if (titleScene->nextStage_ < 0) {
				titleScene->nextStage_ = TitleScene::kMaxStage - 1;
			}

			titleScene->slideDirection_ = -1;

			titleScene->isSliding_ = true;
			titleScene->slideTimer_ = 0.0f;
		} else {
			titleScene->RightArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});
		}

		// Dキーで次のステージ
		if (Input::GetInstance()->TriggerKey(DIK_D)) {

			Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

			titleScene->LeftArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

			titleScene->inputCounter_ = 0.0f;

			titleScene->nextStage_ = titleScene->stageIndex_ + 1;

			if (titleScene->nextStage_ >= TitleScene::kMaxStage) {
				titleScene->nextStage_ = 0;
			}

			titleScene->slideDirection_ = +1;

			titleScene->isSliding_ = true;
			titleScene->slideTimer_ = 0.0f;
		} else {
			titleScene->LeftArrowSprite_->SetColor({1.0f, 1.0f, 1.0f, 0.75f});
		}

		// SPACEキーでゲーム開始
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

			titleScene->inputCounter_ = 0.0f;

			SceneManager::GetInstance()->SetStage(titleScene->stageIndex_);

			// BGMを停止
			if (Audio::GetInstance()->IsPlaying(titleScene->AudioPlayHandle_)) {
				Audio::GetInstance()->StopWave(titleScene->AudioPlayHandle_);
			}

			SceneManager::GetInstance()->ChangeScene("Game");

			return;
		}
	}

	// Bキーでタイトルへ戻る
	if (Input::GetInstance()->TriggerKey(DIK_B) && !titleScene->isSceneChanging_) {

		titleScene->ChangeState(std::make_unique<TitleMainState>());

		Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);
	}
}

// 描画
void StageSelectState::Draw(TitleScene* titleScene) {

	titleScene->titleSceneSprite_->Draw();

	titleScene->LeftArrowSprite_->Draw();
	titleScene->RightArrowSprite_->Draw();
	titleScene->selectStageLogoSprite_->Draw();

	// ステージスライド
	if (titleScene->isSliding_) {

		float t = titleScene->slideTimer_ / titleScene->kSlideDuration_;

		float currentX = 640.0f - (t * 1280.0f * titleScene->slideDirection_);

		float nextX = 640.0f + ((1.0f - t) * 1280.0f * titleScene->slideDirection_);

		// 現在のステージ
		titleScene->stageSprites_[titleScene->stageIndex_]->SetPosition({currentX, 390.0f});
		titleScene->stageSprites_[titleScene->stageIndex_]->Draw();

		// 次のステージ
		titleScene->stageSprites_[titleScene->nextStage_]->SetPosition({nextX, 390.0f});
		titleScene->stageSprites_[titleScene->nextStage_]->Draw();

	} else {

		// 通常時
		titleScene->stageSprites_[titleScene->stageIndex_]->SetPosition({640.0f, 390.0f});
		titleScene->stageSprites_[titleScene->stageIndex_]->Draw();
	}
}