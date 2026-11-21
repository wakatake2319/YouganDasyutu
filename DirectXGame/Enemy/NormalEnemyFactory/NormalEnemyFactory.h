#pragma once
#include "EnemyFactoryBase.h"

class NormalEnemyFactory : public EnemyFactoryBase {
public:
	Enemy* CreateEnemy(KamataEngine::Model* model, KamataEngine::Camera* camera, MapChipField* mapChipField, const KamataEngine::Vector3& position) override;
};