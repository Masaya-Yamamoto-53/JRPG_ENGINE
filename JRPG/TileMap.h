#pragma once
#include <vector>
#include <string>

class TileMap {
private:
    int m_tileWidthNum;   // マップの横方向のタイル数
    int m_tileHeightNum;  // マップの縦方向のタイル数
    std::vector<std::vector<int>> m_tiles;

public:
    TileMap();
    // マップを読み込む
    bool load(const std::string& filePath);
    // タイルIDを取得
    int get(int x, int y) const;
    //マップの横方向のタイル数を取得
    int getTileWidthNum() const;
    //マップの縦方向のタイル数を取得
    int getTileHeightNum() const;
};
