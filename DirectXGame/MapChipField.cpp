#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

using namespace KamataEngine;

// 無名名前空間
namespace {
// マップチップテーブル
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
	{"2", MapChipType::kCoin},
    {"3", MapChipType::kGoal }
};

} // namespace

// マップチップデータをリセット
void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

// 読み込み
void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//  マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// マップチップデータをリセット
	ResetMapChipData();

	std::string line;

	// CSVからマップチップデータを読み込む
	// 高さ方向のループ
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 横方向のループ
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

// マップチップ種別の取得
MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

void MapChipField::SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType type) {

	// 範囲チェック
	if (yIndex >= mapChipData_.data.size()) {
		return;
	}

	if (xIndex >= mapChipData_.data[yIndex].size()) {
		return;
	}

	mapChipData_.data[yIndex][xIndex] = type;
}

// マップチップ座標の取得
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }
uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((position.y + kBlockHeight / 2.0f) / kBlockHeight);

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

// csvに保存する関数
void MapChipField::SaveMapChipCsv(const std::string& filePath) {

	std::ofstream file(filePath);

	if (!file.is_open()) {
		assert(false && "Failed to open file for writing.");
		return;
	}

	for (uint32_t y = 0; y < mapChipData_.data.size(); ++y) {

		for (uint32_t x = 0; x < mapChipData_.data[y].size(); ++x) {

			MapChipType type = mapChipData_.data[y][x];

			std::string value;
			switch (type) {
			case MapChipType::kBlank:
				value = "0";
				break;
			case MapChipType::kBlock:
				value = "1";
				break;
			case MapChipType::kCoin:
				value = "2";
				break;
			case MapChipType::kGoal:
				value = "3";
				break;
			}

			file << value;

			if (x < kNumBlockHorizontal - 1) {
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();
}