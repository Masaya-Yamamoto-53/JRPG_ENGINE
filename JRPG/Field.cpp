#include "Field.h"
#include "GameSettings.h"

Field::Field()
    : m_tileSet()
    , m_tileMap()
{
}

bool Field::load(const std::vector<std::string>& tileFiles
               , const std::string& mapFile
               ) {
    // タイルセット読み込み
    if (!m_tileSet.load(tileFiles)) {
        return false;
    }

    // マップ読み込み
    if (!m_tileMap.load(mapFile)) {
        return false;
    }

    return true;
}

