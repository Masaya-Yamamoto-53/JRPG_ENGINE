#include <fstream>
#include <sstream>
#include "TileMap.h"

TileMap::TileMap() 
    : m_tileWidthNum(0)
    , m_tileHeightNum(0)
{
}

bool TileMap::load(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        return false;
    }

    m_tiles.clear();
    m_tileWidthNum = 0;
    m_tileHeightNum = 0;

    std::string line;
    while (std::getline(ifs, line)) {
        // 空白行をスキップ
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::stringstream ss(line);
        std::string cell;
        std::vector<std::pair<int, int>> row;

        while (std::getline(ss, cell, ',')) {
            try {
                int pos = cell.find('-');
                if (pos != std::string::npos) {
                }
                int pngIndex = std::stoi(cell.substr(0, pos));
                int tileIndex = std::stoi(cell.substr(pos + 1));

                row.push_back(std::make_pair(pngIndex, tileIndex));
            }
            catch (const std::invalid_argument&) {
                return false;
            }
            catch (const std::out_of_range&) {
                return false;
            }

        }
        // 1行目で列数を確定
        if (m_tileWidthNum == 0) {
            m_tileWidthNum = static_cast<int>(row.size());
        }
        else if (static_cast<int>(row.size()) != m_tileWidthNum) {
            return false;
        }
        m_tiles.push_back(row);
    }
    m_tileHeightNum = static_cast<int>(m_tiles.size());

    // 空ファイルは不正
    return m_tileHeightNum > 0;
}

std::pair<int, int> TileMap::get(int x, int y) const {
    if (y < 0 || y >= m_tileHeightNum) { return std::make_pair(-1, -1); }
    if (x < 0 || x >= m_tileWidthNum)  { return std::make_pair(-1, -1); }
    return m_tiles[y][x];
}

int TileMap::getTileWidthNum() const {
    return m_tileWidthNum;
}

int TileMap::getTileHeightNum() const {
    return m_tileHeightNum;
}
