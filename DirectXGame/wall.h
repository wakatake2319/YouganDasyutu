#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class wall {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_;

	// スカイドーム
	wall* wall_ = nullptr;

	float roteSpeed = 0.0002f;
};