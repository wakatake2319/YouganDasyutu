#pragma once
#include "Enemy.h"
#include "KamataEngine.h"

class MapChipField;

class EnemyFactoryBase {
public:
	virtual ~EnemyFactoryBase() = default;

	virtual Enemy* CreateEnemy(KamataEngine::Model* model, KamataEngine::Camera* camera, MapChipField* mapChipField, const KamataEngine::Vector3& position) = 0;
};