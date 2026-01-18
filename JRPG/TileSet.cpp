#include "DxLib.h"
#include "TileSet.h"
#include "GameSettings.h"

TileSet::TileSet()
    : m_tileWidth(GameSettings::instance().getFieldTileWidth())
    , m_tileHeight(GameSettings::instance().getFieldTileHeight())
{
}

bool TileSet::load(const std::vector<std::string>& filePaths) {
    m_tileImages.clear();
    m_tileImages.reserve(filePaths.size());

    for (const auto& path : filePaths) {
        int handle = LoadGraph(path.c_str());
        if (handle == -1) {
            return false;
        }
        m_tileImages.push_back(handle);
    }
    return true;
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
