#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "CameraController.h"
#include "DeatParticles.h"
#include "Goal.h"
#include "Math.h"
#include "Lava.h"
#include "wall.h"
#include "Coin.h"
#include "SceneBase.h"

class GameScene : public SceneBase {
public:
	GameScene(KamataEngine::Input* input) : SceneBase(input) {}

	//~GameScene();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 表示ブロックの生成
	void GenerateBlocks();

	// 表示コインの生成
	void GenerateCoins();

	// 表示ゴールの生成
	void GenerateGoal();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	bool IsFinished() const { return finished_; }

private:
	// ブロック
	KamataEngine::Model* modelBlock_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// カメラ
	Camera camera_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// プレイヤー
	Player* player_;
	Model* player_model_ = nullptr;

	// 溶岩
	Lava* lava_;
	Model* lava_model_ = nullptr;

	// デスパーティクル
	DeathParticles* deathParticles_ = nullptr;
	// デスパーティクルのモデル
	Model* deathParticle_model_ = nullptr;

	// ゴール
	Goal* goal_ = nullptr;
	Model* goal_model_ = nullptr;
	// ゴールの初期化が存在しているか
	bool isGoalInitialized_ = false;

	// 壁
	wall* wall_ = nullptr;
	Model* wall_model_ = nullptr;

	// コイン
	Model* coin_model_ = nullptr;
	// 動的に生成したコインを格納するコンテナ
	std::vector<Coin*> coins_;

	// カメラコントローラー
	CameraController* cameraController_ = nullptr;

	bool mapChanged_ = false;

	// 終了フラグ
	bool finished_ = false;

	// 死亡したかどうかのフラグ
	bool isPlayerDead_ = false;

	// コインを入手したかどうかのフラグ
	bool isCoinGet_ = false;

	// 取得済みコインの数を数える
	int currentGetCoinCount = 0;

	// 前回のコインの個数
	int previousGetCoinCount_ = 0;

	// 溶岩を止めるカウンター
	float isLavaStoppedCounter_ = 0.0f;

	// 溶岩を動かすフラグ
	bool isLavaMoving_ = false;
	bool isFakeLavaMoving_ = false;
	bool isLavaStopped_ = false;

	// BGM
	uint32_t BGMHandle_ = 0;       // BGMを読み込む為の変数
	uint32_t AudioPlayHandle_ = 0; // BGMを再生したり止めたりする変数

	// ステージ
	int stage;

	MapChipType selectedMapChipType_ = MapChipType::kBlock;
};
