#include "CreditState.h"
#include "TitleScene.h"
#include "TitleMainState.h"
#include "KamataEngine.h"

using namespace KamataEngine;

// 更新
void CreditState::Update(TitleScene* titleScene) {

	titleScene->counter_ += 1.0f / 60.0f;

	if (titleScene->counter_ <= TitleScene::kTimeTitleMove) {
		return;
	}

	// Bキーでタイトルへ戻る
	if (Input::GetInstance()->TriggerKey(DIK_B) && !titleScene->isSceneChanging_) {

		titleScene->ChangeState(std::make_unique<TitleMainState>());

		Audio::GetInstance()->PlayWave(titleScene->SEAudioHandle_, false, 0.5f);
	}
}

// 描画
void CreditState::Draw(TitleScene* titleScene) {

	titleScene->titleSceneSprite_->Draw();
	titleScene->creditSceneSprite_->Draw();
}