#pragma once
#include <vector>
#include <string>

class TileSet {
private:
    // タイル画像の配列
    int m_tileWidth;   // タイル1枚の幅
    int m_tileHeight;  // タイル1枚の高さ
    std::vector<int> m_tileImages;

public:
    TileSet();

    bool load(const std::vector<std::string>& filePaths);
    int getTileImage(int tileId) const;
    int getTileWidth() const { return m_tileWidth; }
    int getTileHeight() const { return m_tileHeight; }
};
