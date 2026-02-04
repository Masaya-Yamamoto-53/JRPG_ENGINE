#pragma once
#include <vector>
#include <string>

// JSONから読み込んだタイル定義
struct TileDef {
    int x;
    int y;
    int w;
    int h;
    bool wall;
};

// JSONから読み込んだタイルフレーム定義
struct TileFrame {
    std::string image;
    std::vector<TileDef> frame;
};

// タイルレイヤー情報
struct TileLayer {
    std::vector<int> images;   // タイル画像ハンドル
    std::vector<bool> isWall;  // 壁判定
};

// タイルセットエントリ
struct TileSetEntry {
    // タイルセット内の各アニメーションフレーム
    std::vector<TileLayer> layers;
};

class TileSet {
private:
    int m_tileWidth;   // タイル1枚の幅
    int m_tileHeight;  // タイル1枚の高さ
    int m_frameId;     // 現在のフレーム番号
    std::vector<TileSetEntry> m_entries;  // タイルセットエントリ一覧

    int m_frameCount;
public:
    TileSet();
    ~TileSet();

    std::vector<TileFrame> loadJsonDefs(const std::string& jsonPath) const;

    void addEntry(const TileSetEntry &entry);

    bool loadTileImages(
          const std::vector<TileFrame>& defs
        , const std::string& baseDir
        , TileSetEntry& outEntry
    );

    // タイル画像を読み込む
    bool loadFromJson(const std::vector<std::string>& jsonPaths);
    // 壁判定を取得
    bool isWall(int entryId, int tileId) const;
    // 読み込んだ全タイル画像を開放
    void unload();
    // 指定したtileIdの画像ハンドルを取得する
    int getTileImage(int num, int tileId) const;
    // タイル1枚の幅を取得
    int getTileWidth()  const { return m_tileWidth;  }
    // タイル1枚の高さを取得
    int getTileHeight() const { return m_tileHeight; }
    void setFrameId(int frameId) { m_frameId = frameId; }
    void incrementFrameCounter();
};
