#include "NormalEnemyFactory.h"

Enemy* NormalEnemyFactory::CreateEnemy(Model* model, Camera* camera, MapChipField* mapChipField, const Vector3& position) {
	// 敵を生成
	Enemy* newEnemy = new Enemy();

	// 敵の初期化
	newEnemy->Initialize(model, camera, mapChipField, position);

	// 敵を返す
	return newEnemy;
}