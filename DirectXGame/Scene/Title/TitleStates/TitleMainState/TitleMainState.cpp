#include "TitleMainState.h"
#include "TitleScene.h"
#include "CreditState.h"
#include "GameDescriptionState.h"
#include "StageSelectState.h"

#include "KamataEngine.h"

using namespace KamataEngine;

// 更新
void TitleMainState::Update(TitleScene* titleScene) {

	titleScene->counter_ += 1.0f / 60.0f;

	if (titleScene->counter_ <= TitleScene::kTimeTitleMove) {
		return;
	}

	// Cキーでクレジットへ
	if (Input::GetInstance()->TriggerKey(DIK_C) && !titleScene->isSceneChanging_) {

		titleScene->ChangeState(std::make_unique<CreditState>());

		Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

		return;
	}

	// SPACEキーでステージセレクトへ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !titleScene->isSceneChanging_) {

		titleScene->isSceneChanging_ = true;

		titleScene->ChangeState(std::make_unique<StageSelectState>());

		Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

		return;
	}

	// Tキーでゲーム説明へ
	if (Input::GetInstance()->TriggerKey(DIK_T) && !titleScene->isSceneChanging_) {

		titleScene->ChangeState(std::make_unique<GameDescriptionState>());

		Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);

		return;
	}
}

// 描画
void TitleMainState::Draw(TitleScene* titleScene) {

	titleScene->titleSceneSprite_->Draw();
	titleScene->titleSprite_->Draw();
}