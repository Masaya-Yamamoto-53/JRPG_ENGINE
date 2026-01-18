#pragma once
#include <vector>
#include <string>

class TileSet {
private:
    int m_tileWidth;   // タイル1枚の幅
    int m_tileHeight;  // タイル1枚の高さ
    std::vector<int> m_tileImages;  // タイル画像の配列

public:
    TileSet();

    // タイル画像を読み込む
    bool load(const std::vector<std::string>& filePaths);
    // 指定したtileIdの画像ハンドルを取得する
    int getTileImage(int tileId) const;
    // タイル1枚の幅を取得
    int getTileWidth() const;
    // タイル1枚の高さを取得
    int getTileHeight() const;
};
