#include "EnemySpawner.h"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <imgui.h>

using namespace std;
using namespace KamataEngine;

void EnemySpawner::Initialize(Model* modelEnemy, Camera* camera, MapChipField* mapchipField, std::list<Enemy*>& enemies) {
	modelEnemy_ = modelEnemy;
	camera_ = camera;
	mapchipField_ = mapchipField;
	enemies_ = &enemies;
}
void EnemySpawner::LoadEnemySpawnData(const string& filePath) {

	enemySpawnData_.clear();

	ifstream file(filePath);

#ifdef DEBUG
	assert(file.is_open());
#endif

	string line;

	while (getline(file, line)) {

		// 空行を無視
		if (line.empty()) {
			continue;
		}

		// コメント行を無視
		if (line.find("//") == 0) {
			continue;
		}

		stringstream lineStream(line);

		string word;

		// POP
		getline(lineStream, word, ',');

		if (word != "POP") {
			continue;
		}

		EnemySpawnData data;

		// X
		getline(lineStream, word, ',');
		data.position.x = static_cast<float>(atof(word.c_str()));

		// Y
		getline(lineStream, word, ',');
		data.position.y = static_cast<float>(atof(word.c_str()));

		// Z
		getline(lineStream, word, ',');
		data.position.z = static_cast<float>(atof(word.c_str()));

		// VX
		getline(lineStream, word, ',');
		data.moveVector.x = static_cast<float>(atof(word.c_str()));

		// VY
		getline(lineStream, word, ',');
		data.moveVector.y = static_cast<float>(atof(word.c_str()));

		// VZ
		getline(lineStream, word, ',');
		data.moveVector.z = static_cast<float>(atof(word.c_str()));

		enemySpawnData_.push_back(data);
	}

	file.close();
}
void EnemySpawner::CreateEnemiesFromSpawnData() {

	// 既存の敵を削除
	for (Enemy* enemy : *enemies_) {
		delete enemy;
	}

	enemies_->clear();

	for (const EnemySpawnData& data : enemySpawnData_) {

		Enemy* newEnemy = new Enemy();

		newEnemy->Initialize(modelEnemy_, camera_, mapchipField_, Vector3(data.position.x, data.position.y, data.position.z));

		newEnemy->SetPosition(data.position);
		newEnemy->SetMoveVector(data.moveVector);

		enemies_->push_back(newEnemy);
	}
}
void EnemySpawner::SaveEnemySpawnData(const string& filePath) {

	ofstream file(filePath);

	// ファイルが開けなかった場合
	if (!file.is_open()) {
		OutputDebugStringA(("CSV保存失敗: " + filePath + "\n").c_str());
		return;
	}

#ifdef DEBUG
	assert(file.is_open());
#endif

	file << "// X座標,Y座標,Z座標,X方向,Y方向,Z方向\n";

	for (const EnemySpawnData& data : enemySpawnData_) {

		file << "POP," << data.position.x << "," << data.position.y << "," << data.position.z << "," << data.moveVector.x << "," << data.moveVector.y << "," << data.moveVector.z << "\n";
	}

	file.close();

	OutputDebugStringA(("CSV保存成功: " + filePath + "\n").c_str());
}

void EnemySpawner::DrawEnemyEditor() {

#ifdef USE_IMGUI

	ImGui::Begin("Enemy Editor");

	for (int i = 0; i < enemySpawnData_.size(); i++) {

		ImGui::PushID(i);

		ImGui::Text("Enemy %d", i);

		ImGui::DragFloat3("Position", &enemySpawnData_[i].position.x, 0.1f);

		ImGui::DragFloat3("Move Vector", &enemySpawnData_[i].moveVector.x, 0.01f);

		// 削除
		if (ImGui::Button("Remove")) {

			enemySpawnData_.erase(enemySpawnData_.begin() + i);

			ImGui::PopID();

			break;
		}

		ImGui::Separator();

		ImGui::PopID();
	}

	// 敵追加
	if (ImGui::Button("Add Enemy")) {

		EnemySpawnData data;

		data.position = {0.0f, 0.0f, 0.0f};
		data.moveVector = {0.01f, 0.0f, 0.0f};

		enemySpawnData_.push_back(data);
	}

	// 変更をゲーム内に反映
	if (ImGui::Button("Apply")) {
		CreateEnemiesFromSpawnData();
	}

	// 保存
	if (ImGui::Button("Save")) {
		if (!GetEnemyFile().empty()) {
			SaveEnemySpawnData(GetEnemyFile());
		}
	}

	ImGui::End();

#endif
}

void EnemySpawner::SetEnemyFile(const string& filePath) { currentEnemyFile_ = filePath; }

const string& EnemySpawner::GetEnemyFile() const { return currentEnemyFile_; }