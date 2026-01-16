#pragma once
#include <vector>
#include <string>

class TileMap {
private:
    int m_tileWidthNum = 0;
    int m_tileHeightNum = 0;
    std::vector<std::vector<int>> m_tiles;

public:
    TileMap();

    // マップを読み込む
    bool load(const std::string& filePath);

    // タイルIDを取得
    int get(int x, int y) const;

    int getTileWidthNum() const { return m_tileWidthNum; }
    int getTileHeightNum() const { return m_tileHeightNum; }
};
