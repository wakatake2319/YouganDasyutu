#pragma once

#define NOMINMAX

#include "Enemy.h"
#include "KamataEngine.h"
#include "Player.h"
#include "EnemyFactoryBase.h"
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace KamataEngine;

class MapChipField;

class EnemySpawner {

public:
	// 敵配置データ
	struct EnemySpawnData {
		Vector3 position;
		Vector3 moveVector;
	};

	// 初期化
	void Initialize(Model* modelEnemy, Camera* camera, MapChipField* mapchipField, std::list<Enemy*>& enemies);

	// CSV読み込み
	void LoadPopData(const std::string& filePath);

	// 更新
	void Update();

	// 敵配置データの読み込み
	void LoadEnemySpawnData(const std::string& filePath);

	// 敵配置データの保存
	void SaveEnemySpawnData(const std::string& filePath);

	// 敵配置データから生成
	void CreateEnemiesFromSpawnData();

	// ImGui
	void DrawEnemyEditor();

	void SetEnemyFile(const std::string& filePath);

	// 敵配置のゲッター
	const std::string& GetEnemyFile() const;

	// 敵配置データのセッター
	void SetEnemyFactory(EnemyFactoryBase* factory);

private:
	// POP処理
	void ExecutePop(std::stringstream& lineStream);

	// WAIT処理
	void ExecuteWait(std::stringstream& lineStream);

	// 敵配置データ
	std::vector<EnemySpawnData> enemySpawnData_;

	// CSVの読み込み
	std::stringstream popCommands_;

	// モデル
	Model* modelEnemy_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;


	// 敵リスト
	std::list<Enemy*>* enemies_ = nullptr;

	// 待機処理
	bool isWaiting_ = false;
	int32_t waitTimer_ = 0;

	// マップチップフィールド
	MapChipField* mapchipField_ = nullptr;
	
	std::string currentEnemyFile_;
	EnemyFactoryBase* enemyFactory_ = nullptr;
};