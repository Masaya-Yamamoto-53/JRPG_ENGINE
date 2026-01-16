#include <fstream>
#include <sstream>
#include "TileMap.h"

TileMap::TileMap() {
}

bool TileMap::load(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        return false;
    }

    m_tiles.clear();
    m_width = 0;
    m_height = 0;

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cell;

        std::vector<int> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }
        if (m_width == 0) {
            m_width = static_cast<int>(row.size());
        }
        m_tiles.push_back(row);
    }
    m_height = static_cast<int>(m_tiles.size());
    return true;
}

int TileMap::get(int x, int y) const {
    if (y < 0 || y >= m_height) return -1;
    if (x < 0 || x >= m_width) return -1;
    return m_tiles[y][x];
}