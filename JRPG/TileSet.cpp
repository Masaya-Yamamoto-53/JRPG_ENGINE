#include "DxLib.h"
#include "TileSet.h"
#include "GameSettings.h"

TileSet::TileSet()
    : m_tileWidth(GameSettings::instance().getFieldTileWidth())
    , m_tileHeight(GameSettings::instance().getFieldTileHeight())
{
}

TileSet::~TileSet() {
    unload(); // 保持している画像ハンドルを全て開放
}

bool TileSet::load(const std::vector<std::string>& filePaths) {
    // 既存の画像を開放
    unload();
    // メモリ再確保を避けるため、必要数を予約
    m_tileImages.reserve(filePaths.size());

    for (const auto& path : filePaths) {
        int handle = LoadGraph(path.c_str());
        if (handle == -1) {
            // 途中で失敗s他場合は、
            // 読み込んだ分を開放する
            unload();
            return false;
        }
        m_tileImages.push_back(handle);
    }
    return true;
}

void TileSet::unload() {
    for (int handle : m_tileImages) {
        if (handle != -1) {
            // 保持している画像ハンドルを開放
            DeleteGraph(handle);
        }
    }
    m_tileImages.clear();
}

int TileSet::getTileImage(int tileId) const {
    if (tileId < 0 || tileId >= static_cast<int>(m_tileImages.size())) {
        return -1;
    }
    return m_tileImages[tileId];
}

int TileSet::getTileWidth() const {
    return m_tileWidth;
}

int TileSet::getTileHeight() const {
    return m_tileHeight;
}
