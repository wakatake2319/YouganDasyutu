#include "SceneManager.h"
#include "KamataEngine.h"
#include <Windows.h>
#include "imgui.h"

using namespace KamataEngine;


//// シーン
//enum class Scene {
//	kUnknown = 0,
//	kTitle, // タイトル
//	kGame,  // ゲーム
//	kClear, // クリア
//	kGameOver, // ゲームオーバー
//};
//
//// 現在のシーン
//Scene scene = Scene::kUnknown;
//
//// ==========================
//// シーン切り替え処理
//// ==========================
//void ChangScene() {
//	switch (scene) {
//	case Scene::kTitle:
//		if (titleScene->IsFinished()) {
//			// シーン変更
//			scene = Scene::kGame;
//
//			// 旧シーンの開放
//			delete titleScene;
//			titleScene = nullptr;
//
//			// 新シーンの生成と初期化
//			gameScene = new GameScene();
//			gameScene->Initialize();
//		}
//		break;
//	case Scene::kGame:
//		if (gameScene && gameScene->IsFinished()) {
//			auto result = gameScene->GetResult(); // 先に取得
//
//			delete gameScene; // ← ここで消す
//			gameScene = nullptr;
//
//			if (result == GameScene::Result::kDead) {
//				// 死亡した → ゲームオーバーへ
//
//				scene = Scene::kGameOver;
//				gameoverScene = new GameOverScene();
//				gameoverScene->Initialize();
//			}
//
//			else if (result == GameScene::Result::kClear) {
//				scene = Scene::kClear;
//				clearScene = new ClearScene();
//				clearScene->Initialize();
//			}
//		}
//		break;
//	case Scene::kClear:
//		if (clearScene->IsFinished()) {
//			// シーン変更
//			scene = Scene::kTitle;
//			// 旧シーンの開放
//			delete clearScene;
//			clearScene = nullptr;
//			// 新シーンの生成と初期化
//			titleScene = new TitleScene();
//			titleScene->Initialize();
//		}
//		break;
//	case Scene::kGameOver:
//		if (gameoverScene->IsFinished()) {
//			// シーン変更
//			scene = Scene::kTitle;
//			// 旧シーンの開放
//			delete gameoverScene;
//			gameoverScene = nullptr;
//			// 新シーンの生成と初期化
//			titleScene = new TitleScene();
//			titleScene->Initialize();
//		}
//		break;
//	}
//}
//
//// ==========================
//// シーンの更新
//// ==========================
//void UpdateScene() {
//	switch (scene) {
//	case Scene::kTitle:
//		titleScene->Update();
//		break;
//	case Scene::kGame:
//		gameScene->Update();
//		break;
//	case Scene::kClear:
//		clearScene->Update();
//		break;
//	case Scene::kGameOver:
//		gameoverScene->Update();
//		break;
//	}
//}
//
//// ==========================
//// シーンの描画
//// ==========================
//void DrawScene() {
//	switch (scene) {
//	case Scene::kTitle:
//		titleScene->Draw();
//		break;
//	case Scene::kGame:
//		gameScene->Draw();
//		break;
//	case Scene::kClear:
//		clearScene->Draw();
//		break;
//	case Scene::kGameOver:
//		gameoverScene->Draw();
//		break;
//	}
//}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化処理
	Initialize(L"LE3D_14_タケウチ_ハルカ_溶岩脱出");



	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// imguiManagerインスタンスの取得
	ImGuiManager* imguiManager_ = ImGuiManager::GetInstance();

	//// 最初のシーンの初期化
	//scene = Scene::kTitle;
	//titleScene = new TitleScene;
	//titleScene->Initialize();

	// sceneManagerの初期化
	SceneManager::GetInstance()->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}
#ifdef _DEBUG
		// imguiの受付開始
		imguiManager_->Begin();
#endif

		// シーンの切り替え
		//ChangScene();
		// 現在シーンの更新
		//UpdateScene();

		// sceneManagerの更新
		SceneManager::GetInstance()->Update();



#ifdef _DEBUG
		// imguiの受付終了
		imguiManager_->End();
#endif

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		//DrawScene();

		// sceneManagerの描画
		SceneManager::GetInstance()->Draw();

		// imguiの描画
		imguiManager_->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	//// タイトルシーンの開放
	//delete titleScene;
	//// ゲームシーンの開放
	//delete gameScene;
	//// クリアシーンの開放
	//delete clearScene;
	//// ゲームオーバーシーンの開放
	//delete gameoverScene;

	// sceneManagerの終了処理
	SceneManager::GetInstance()->Finalize();

	// エンジンの終了処理
	Finalize();

	return 0;
}
