#include "GameScene.h"
#include <cassert>
#include "SceneManager.h"


using namespace KamataEngine;

// 終了
void GameScene::Finalize() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	delete mapChipField_;
	delete modelBlock_;
	delete debugCamera_;
	delete player_;
	delete cameraController_;
	delete deathParticles_;
	delete deathParticle_model_;
	delete goal_;
	delete goal_model_;
	delete lava_;
	delete lava_model_;
	delete player_model_;
	delete wall_;
	delete wall_model_;
	delete coin_model_;
	for (Coin* coin : coins_) {
		delete coin;
	}

	coins_.clear();
}

void GameScene::Initialize() {
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	camera_.Initialize();

#pragma region ステージの取得
	stage = SceneManager::GetInstance()->GetStage();
	std::string csv;

	stage = SceneManager::GetInstance()->GetStage();

	switch (stage) {
	case 0:
		csv = "Resources/map/map1.csv";
		break;

	case 1:
		csv = "Resources/map/map2.csv";
		break;

	case 2:
		csv = "Resources/map/map3.csv";
		break;
	case 3:
		csv = "Resources/map/map4.csv";
		break;
	case 4:
		csv = "Resources/map/map5.csv";
		break;
	}

#pragma endregion

#pragma region マップチップの初期化
	// ブロックモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	// マップチップの初期化
	mapChipField_ = new MapChipField;
	//mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	mapChipField_->LoadMapChipCsv(csv);
	GenerateBlocks();
#pragma endregion

#pragma region プレイヤーの初期化
	// プレイヤーの初期化
	player_ = new Player();
	// プレイヤーのモデル
	player_model_ = Model::CreateFromOBJ("player");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 6);

	player_->SetMapChipField(mapChipField_);
	player_->Initialize(player_model_, &camera_, playerPosition);
#pragma endregion

#pragma region 溶岩の初期化

	lava_ = new Lava();

	lava_model_ = Model::CreateFromOBJ("lava");
	Vector3 lavaPosition = mapChipField_->GetMapChipPositionByIndex(15, 1);
	lava_->Initialize(lava_model_, &camera_, lavaPosition);
#pragma endregion

#pragma region デスパーティクル
	// モデル読み込み
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");
#pragma endregion

#pragma region カメラコントロール
	cameraController_ = new CameraController;
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	// カメラコントローラーの移動範囲の指定
	CameraController::Rect cameraArea = {
	    11.0f, MapChipField::GetNumBlockHorizontal() * MapChipField::kBlockWidth - 12.0f,
	    6.0f,                                                                      // 下の範囲
	    MapChipField::GetNumBlockVirtical() * MapChipField::kBlockHeight - 4.0f}; // 上の範囲
	cameraController_->SetMovableArea(cameraArea);
#pragma endregion

#pragma region ゴール
	goal_ = new Goal();

	goal_model_ = Model::CreateFromOBJ("Goal");

	GenerateGoal();
#pragma endregion

#pragma region BGM
	// BGMの読み込み
	BGMHandle_ = Audio::GetInstance()->LoadWave("audio/BGM/BGM1.wav");
	AudioPlayHandle_ = Audio::GetInstance()->PlayWave(BGMHandle_, true, 0.2f);
#pragma endregion

#pragma region 壁
	// 壁の初期化
	wall_ = new wall();
	wall_model_ = Model::CreateFromOBJ("wall");
	Vector3 wallPosition = mapChipField_->GetMapChipPositionByIndex(0, 99);

	wall_->Initialize(wall_model_, &camera_, wallPosition);
#pragma endregion

#pragma region コイン
	coin_model_ = Model::CreateFromOBJ("Coin");
	GenerateCoins();
#pragma endregion

	// 自キャラの座標を取得
	const Vector3& deathParticlesPosition = player_->GetWorldPosition();
	// デスパーティクルの初期化
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
}

// 表示ブロックの生成
void GameScene::GenerateBlocks() {
	for (auto& line : worldTransformBlocks_) {
		for (WorldTransform*& wt : line) {
			delete wt;
			wt = nullptr;
		}
	}

	worldTransformBlocks_.clear();

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の総素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

// 表示コインの生成
void GameScene::GenerateCoins() {
	// 既存のコインを削除
	for (Coin* coin : coins_) {
		delete coin;
	}

	coins_.clear();

	// コインの生成
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVirtical(); ++i) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kCoin) {
				continue;
			}

			Vector3 position = mapChipField_->GetMapChipPositionByIndex(j, i);

			Coin* coin = new Coin();

			coin->Initialize(coin_model_, &camera_, position);

			coins_.push_back(coin);
		}
	}
}

// 表示ゴールの生成
void GameScene::GenerateGoal() {
	// ゴールの生成
	for (uint32_t i = 0; i < mapChipField_->GetNumBlockVirtical(); ++i) {
		for (uint32_t j = 0; j < mapChipField_->GetNumBlockHorizontal(); ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) != MapChipType::kGoal) {
				continue;
			}
			Vector3 position = mapChipField_->GetMapChipPositionByIndex(j, i);
			goal_->Initialize(goal_model_, &camera_, position);
			
			isGoalInitialized_ = true;

			return; // ゴールは1つしかないので、見つけたら終了
		}
	}
}

void GameScene::Update() {

	// プレイヤーの更新
	player_->Update();

	// ゴールの更新
	if (goal_ && isGoalInitialized_) {
		switch (stage) {
		case 0:
			if (currentGetCoinCount >= 2) {
				goal_->Update();
			}
			break;

		case 1:
			if (currentGetCoinCount >= 3) {
				goal_->Update();
			}
			break;

		case 2:
			if (currentGetCoinCount >= 4) {
				goal_->Update();
			}
			break;

		case 3:
			if (currentGetCoinCount >= 6) {
				goal_->Update();
			}
			break;
		case 4:
			if (currentGetCoinCount >= 7) {
				goal_->Update();
			}
			break;
		}
	}


	// 壁の更新
	wall_->Update();

	// コインの更新
	for (Coin* coin : coins_) {
		if (coin) {
			coin->Update();
		}
	}

#ifdef _DEBUG

#pragma region デバッグカメラの更新
	ImGui::Begin("Debug Camera");

	ImGui::Checkbox("Debug Camera Active", &isDebugCameraActive_);

	ImGui::End();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		//if (Input::GetInstance()->PushKey(DIK_LSHIFT)) {
			debugCamera_->Update();
		//}
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
		cameraController_->Update();

	}
#pragma endregion
#ifdef _DEBUG

#pragma region マップチップの変更
#pragma region マップチップの種類選択
	ImGui::Begin("Map Chip Type");

	if (ImGui::Button("Block")) {
		selectedMapChipType_ = MapChipType::kBlock;
	}

	ImGui::SameLine();

	if (ImGui::Button("Coin")) {
		selectedMapChipType_ = MapChipType::kCoin;
	}

	ImGui::SameLine();

	if (ImGui::Button("Goal")) {
		selectedMapChipType_ = MapChipType::kGoal;
	}

	ImGui::SameLine();

	if (ImGui::Button("Blank")) {
		selectedMapChipType_ = MapChipType::kBlank;
	}

	ImGui::End();
#pragma endregion

	ImGui::Begin("Map Editor");
	for (uint32_t y = 0; y < MapChipField::GetNumBlockVirtical(); y++) {

		for (uint32_t x = 0; x < MapChipField::GetNumBlockHorizontal(); x++) {

			MapChipType type = mapChipField_->GetMapChipTypeByIndex(x, y);

			const char* label = "";

			ImGui::PushID(y * 1000 + x);

			// imguiの色の設定
			switch (type) {

			case MapChipType::kBlock:
				// ブロック：黒
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
				break;

			case MapChipType::kCoin:
				// コイン：黄色
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 0.2f, 1.0f));
				break;

			case MapChipType::kGoal:
				// ゴール：青
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
				break;

			case MapChipType::kBlank:
				// 空白：白
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
				break;
			}

			if (ImGui::Button(label, ImVec2(10, 10))) {

				mapChipField_->SetMapChipTypeByIndex(x, y, selectedMapChipType_);

				mapChanged_ = true;
			}

			// 色を元に戻す
			ImGui::PopStyleColor(3);

			ImGui::PopID();

			ImGui::SameLine();
		}

		ImGui::NewLine();
	}

	ImGui::End();

	ImGui::Begin("Map Change");

	// CSV保存と読み込みのボタン
	if (ImGui::Button("Save CSV")) {
		if (stage == 0) {
			mapChipField_->SaveMapChipCsv("Resources/map/map1.csv");
		} else if (stage == 1) {
			mapChipField_->SaveMapChipCsv("Resources/map/map2.csv");
		} else if (stage == 2) {
			mapChipField_->SaveMapChipCsv("Resources/map/map3.csv");
		} else if (stage == 3) {
			mapChipField_->SaveMapChipCsv("Resources/map/map4.csv");
		} else if (stage == 4) {
			mapChipField_->SaveMapChipCsv("Resources/map/map5.csv");
		}
		//mapChipField_->SaveMapChipCsv("Resources/blocks.csv");
	}

	if (ImGui::Button("Reload CSV")) {
		if (stage == 0) {
			mapChipField_->LoadMapChipCsv("Resources/map/map1.csv");
		} else if (stage == 1) {
			mapChipField_->LoadMapChipCsv("Resources/map/map2.csv");
		} else if (stage == 2) {
			mapChipField_->LoadMapChipCsv("Resources/map/map3.csv");
		} else if (stage == 3) {
			mapChipField_->LoadMapChipCsv("Resources/map/map4.csv");
		} else if (stage == 4) {
			mapChipField_->LoadMapChipCsv("Resources/map/map5.csv");
		}
		//mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
		GenerateBlocks();
		GenerateCoins();
		GenerateGoal();
	}

	ImGui::End();

	// マップチップの変更があった場合、ブロックを再生成
	if (mapChanged_) {
		GenerateBlocks(); // ブロックを再生成
		GenerateCoins();  // コインを再生成
		GenerateGoal();   // ゴールを再生成
		mapChanged_ = false;
	}

#pragma endregion

	ImGui::Begin("LavaPosition");
	ImGui::Text("Lava Position: (%.2f, %.2f, %.2f)", lava_->GetWorldPosition().x, lava_->GetWorldPosition().y, lava_->GetWorldPosition().z);
	ImGui::End();

	ImGui::Begin("PlayerPosition");
	ImGui::Text("Player Position: (%.2f, %.2f, %.2f)", player_->GetWorldPosition().x, player_->GetWorldPosition().y, player_->GetWorldPosition().z);
	ImGui::End();

#endif

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換~DirectXに転送
			WorldTransformUpdate(*worldTransformBlock);
		}
	}

	// プレイヤーがある程度下まで来た場合、溶岩を動かす
	if (player_->GetWorldPosition().y < 90.0f) {
		isLavaMoving_ = true;
	}  

	if (lava_->GetWorldPosition().y >= 95.5f) {
		isFakeLavaMoving_ = true;
	} else {
		isFakeLavaMoving_ = false;
	}

	// 取得済みコインの数を数える
	currentGetCoinCount = 0;

	for (Coin* coin : coins_) {
		if (!coin) {
			continue;
		}

		if (coin->IsGet()) {
			// 取得済みコインの数をカウント
			currentGetCoinCount++;
		}
	}

	// コインを取得した「瞬間」
	if (currentGetCoinCount > previousGetCoinCount_) {
		isCoinGet_ = true;
		isLavaStopped_ = true;
		isLavaStoppedCounter_ = 0.0f;
	}

	// 前フレームの状態を保存
	previousGetCoinCount_ = currentGetCoinCount;

	if (isCoinGet_) {
		isLavaStoppedCounter_ += 1.0f / 60.0f;
	} 

	switch (stage) {
	case 0:
		if (isLavaStoppedCounter_ > 0.5f) {
			isLavaStopped_ = false;
			isCoinGet_ = false;
			isLavaStoppedCounter_ = 0.0f;
		}
		break;

	case 1:
		if (isLavaStoppedCounter_ > 1.0f) {
			isLavaStopped_ = false;
			isCoinGet_ = false;
			isLavaStoppedCounter_ = 0.0f;
		}
		break;

	case 2:
		if (isLavaStoppedCounter_ > 2.0f) {
			isLavaStopped_ = false;
			isCoinGet_ = false;
			isLavaStoppedCounter_ = 0.0f;
		}
		break;
	case 3:
		if (isLavaStoppedCounter_ > 0.5f) {
			isLavaStopped_ = false;
			isCoinGet_ = false;
			isLavaStoppedCounter_ = 0.0f;
		}
		break;
	case 4:
		if (isLavaStoppedCounter_ > 0.075f) {
			isLavaStopped_ = false;
			isCoinGet_ = false;
			isLavaStoppedCounter_ = 0.0f;
		}
		break;
	}

	if (player_->IsGoalGet()) {
		isLavaStopped_ = true;
	}

	if (isLavaMoving_ || isFakeLavaMoving_) {
		if (!isLavaStopped_){
			// 溶岩の更新
			lava_->Update();
		}
	}


	// 全ての当たり判定
	CheckAllCollisions();

	if (player_->IsDeath()) {

		assert(deathParticles_ != nullptr);
		if (!isPlayerDead_) {
			isPlayerDead_ = true;
			deathParticles_->StartPosition(player_->GetWorldPosition());
		}

		deathParticles_->Update();

		if (deathParticles_->IsFinished()) {
			// ゲームオーバーシーンに遷移
			SceneManager::GetInstance()->ChangeScene("GameOver");
			// 音を止める
			if (Audio::GetInstance()->IsPlaying(AudioPlayHandle_)) {
				// BGM停止
				Audio::GetInstance()->StopWave(AudioPlayHandle_);
			}
		}

	}

	// クリア演出の更新
	// クリア演出が終了したらフェードアウトフェーズへ移行
	if (goal_ && goal_->IsFinished() && isGoalInitialized_) {

		// 音を止める
		if (Audio::GetInstance()->IsPlaying(AudioPlayHandle_)) {
			// BGM停止
			Audio::GetInstance()->StopWave(AudioPlayHandle_);
		}
		// ゲームクリアシーンに遷移
		SceneManager::GetInstance()->ChangeScene("Clear");
	}
}

void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	// DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	assert(modelBlock_);

	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	// ブロック描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// プレイヤー描画
	if (!player_->IsDeath()) {
		player_->Draw();
	}
	// ゴールの描画
	if (goal_ && !goal_->IsGet() && isGoalInitialized_) {
		if (currentGetCoinCount >= 6) {
			goal_->Draw();
		}
	}

	// デスパーティクル描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// コインの描画
	for (Coin* coin : coins_) {
		if (!coin) {
			continue;
		}

		if (!coin->IsGet()) {
			coin->Draw();
		}
	}

	// 溶岩の描画
	lava_->Draw();
	// 壁の描画
	wall_->Draw();

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw();


	// スプライト描画後処理
	Sprite::PostDraw();

}

void GameScene::CheckAllCollisions() {
	AABB aabb1, aabb2;
#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// ==============================
		// 自キャラと鍵の当たり判定
		// ==============================
		{
			aabb1 = player_->GetAABB();
			aabb2 = goal_->GetAABB();

			if (IsCollision(aabb1, aabb2)) {

				player_->OnCollisionGoal(goal_);
				goal_->OnCollision(player_);
			}
		}

		// ==============================
		// 自キャラと溶岩の当たり判定
		// ==============================
		{
			aabb1 = player_->GetAABB();
			aabb2 = lava_->GetAABB();
			if (IsCollision(aabb1, aabb2) || Input::GetInstance()->TriggerKey(DIK_P)) {
				player_->OnCollisionLava(lava_);
				lava_->OnCollision();
			}
		}

		// ==============================
		// 自キャラとコインの当たり判定
		// ==============================
		{
			aabb1 = player_->GetAABB();
			for (Coin* coin : coins_) {

				if (!coin) {
					continue;
				}

				// すでに取得済みなら判定しない
				if (coin->IsGet()) {
					continue;
				}

				aabb2 = coin->GetAABB();

				if (IsCollision(aabb1, aabb2)) {
					player_->OnCollisionCoin(coin);
					coin->OnCollision(player_);
				}
			}
		}
	}
#pragma endregion
}
