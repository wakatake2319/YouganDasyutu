#pragma once
#include "KamataEngine.h"
// #include "Vector3.h"
#include <vector>

enum class MapChipType {
	// 空白
	kBlank,
	// ブロック
	kBlock,
	// コイン
	kCoin,
	// ゴール
	kGoal,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
	// サイズ
	std::vector<std::vector<MapChipType>> size;
};

class MapChipField {
public:
	// 座標からマップチップ番号を計算
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// ブロックの範囲
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

	// マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	// マップチップ種別の設定
	void SetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex, MapChipType type);

	// マップチップ座標の取得
	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	static uint32_t GetNumBlockVirtical();
	static uint32_t GetNumBlockHorizontal();

	// 座標からマップチップ番号を計算
	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	// ブロック範囲取得関数
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	// csvに保存する関数
	void SaveMapChipCsv(const std::string& filePath);

private:
	MapChipData mapChipData_;

	// ブロックの個数
	// 縦方向のブロック数
	static inline const uint32_t kNumBlockVirtical = 100;
	// 横方向のブロック数
	static inline const uint32_t kNumBlockHorizontal = 30;
};
