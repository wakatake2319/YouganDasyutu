#include "ClearScene.h"
#include "math.h"
#include <numbers>
#include "SceneManager.h"


void ClearScene::Finalize() {
	delete clearSceneSprite_;
}
void ClearScene::Initialize() {

	clearSceneTH_ = TextureManager::Load("SceneTexture/GameClear.png");
	clearSceneSprite_ = Sprite::Create(clearSceneTH_, Vector2(0.0f, 0.0f));
	clearSceneSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// カメラ初期化
	camera_.Initialize();

	// BGM
	BGMHandle_ = Audio::GetInstance()->LoadWave("audio/BGM/GameClear.wav");
	AudioPlayHandle_ = Audio::GetInstance()->PlayWave(BGMHandle_, false, 0.5f);

	// SEの読み込み
	// 決定音
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/kettei.wav");

	Audio::GetInstance()->SetVolume(AudioPlayHandle_, 0.2f);

	isSceneChanging_ = false;
}
void ClearScene::Update() {

	counter_ += 1.0f / 60.0f;
	// カウンターが1秒を超えたら、スペースキーを押すとタイトルに戻るようにする
	if (counter_ > kTimeClearMove) {
		// スペースキーが押されたらタイトルに戻る

		if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !isSceneChanging_) {

			// シーン切り替え中にする
			isSceneChanging_ = true;

			// 効果音を鳴らす
			Audio::GetInstance()->PlayWave(SEAudioHandle_, false, 0.5f);
			// 音を止める
			if (Audio::GetInstance()->IsPlaying(AudioPlayHandle_)) {
				Audio::GetInstance()->StopWave(AudioPlayHandle_);
			}
			// タイトルに戻る
			SceneManager::GetInstance()->ChangeScene("Title");
		}

		// if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		//	finished_ = true;
		// }

		// float angle = counter_ / kTimeClearMove * 2.0f * std::numbers::pi_v<float>;

		camera_.TransferMatrix();
	}
}

void ClearScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	// クリアシーンのテクスチャを描画
	clearSceneSprite_->Draw();

	Sprite::PostDraw();
}