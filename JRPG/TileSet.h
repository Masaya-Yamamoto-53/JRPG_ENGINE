#pragma once
#include <vector>
#include <string>

struct TileDef {
    std::string image;
    int x;
    int y;
    int w;
    int h;
    bool wall;
};

class TileSet {
private:
    int m_tileWidth;   // タイル1枚の幅
    int m_tileHeight;  // タイル1枚の高さ
    std::vector<int> m_tileImages;  // タイル画像の配列
    std::vector<bool> m_isWall;     // 壁判定

public:
    TileSet();
    ~TileSet();

    // タイル画像を読み込む
    bool loadFromJson(const std::vector<std::string>& jsonPaths);
    // 壁判定を取得
    bool isWall(int tileId) const;
    // 読み込んだ全タイル画像を開放
    void unload();
    // 指定したtileIdの画像ハンドルを取得する
    int getTileImage(int tileId) const;
    // タイル1枚の幅を取得
    int getTileWidth() const;
    // タイル1枚の高さを取得
    int getTileHeight() const;
};
