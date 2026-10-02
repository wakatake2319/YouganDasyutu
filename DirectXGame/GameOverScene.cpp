#include "GameOverScene.h"
#include "SceneManager.h"
#include "math.h"
#include <numbers>

void GameOverScene::Finalize() { delete gameoverSceneSprite_; }
void GameOverScene::Initialize() {

	gameoverSceneTH_ = TextureManager::Load("SceneTexture/GameOver.png");
	gameoverSceneSprite_ = Sprite::Create(gameoverSceneTH_, Vector2(0.0f, 0.0f));
	gameoverSceneSprite_->SetSize(Vector2(1280.0f, 720.0f));

	// カメラ初期化
	camera_.Initialize();

	const float kPlayerGameOver = 2.0f;

	worldTransformGameOver_.Initialize();

	worldTransformGameOver_.scale_ = {kPlayerGameOver, kPlayerGameOver, kPlayerGameOver};

	const float kPlayerScale = 10.0f;

	worldTransformPlayer_.Initialize();

	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};

	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;

	worldTransformPlayer_.translation_.x = -2.0f;

	worldTransformPlayer_.translation_.y = -10.0f;

	// BGM
	BGMHandle_ = Audio::GetInstance()->LoadWave("audio/BGM/deth.wav");
	AudioPlayHandle_ = Audio::GetInstance()->PlayWave(BGMHandle_, false, 0.5f);

	// SEの読み込み
	// 決定音
	SEAudioHandle_ = Audio::GetInstance()->LoadWave("audio/SE/kettei.wav");

	Audio::GetInstance()->SetVolume(AudioPlayHandle_, 0.2f);
}
void GameOverScene::Update() {
	counter_ += 1.0f / 60.0f;
	if (counter_ > kTimeGameOverMove) {

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

		// counter_ = std::fmod(counter_, kTimeGameOverMove);

		// float angle = counter_ / kTimeGameOverMove * 2.0f * std::numbers::pi_v<float>;

		// worldTransformGameOver_.translation_.y = std::sin(angle) + 10.0f;

		camera_.TransferMatrix();
	}
}

void GameOverScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	// ゲームオーバーシーンのテクスチャを描画
	gameoverSceneSprite_->Draw();

	Sprite::PostDraw();
}