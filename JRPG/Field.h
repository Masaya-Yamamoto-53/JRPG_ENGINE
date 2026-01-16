#pragma once
#include "TileSet.h"
#include "TileMap.h"

class Field {
private:
    TileSet m_tileSet;
    TileMap m_tileMap;
public:
    Field();

    // ステージデータを読み込む
    bool load(const std::vector<std::string>& tileFiles
            , const std::string& mapFile);

    const TileSet& getTileSet() const { return m_tileSet; }
    const TileMap& getTileMap() const { return m_tileMap; }
};
