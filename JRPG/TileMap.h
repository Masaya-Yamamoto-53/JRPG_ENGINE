#pragma once
#include <vector>
#include <string>

class TileMap {
private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::vector<int>> m_tiles;

public:
    TileMap();

    // マップを読み込む
    bool load(const std::string& filePath);

    // タイルIDを取得
    int get(int x, int y) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
};
