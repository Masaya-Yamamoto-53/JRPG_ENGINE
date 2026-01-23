#include <fstream>
#include "DxLib.h"
#include "TileSet.h"
#include "GameSettings.h"

#include "json.hpp"
using json = nlohmann::json;

TileSet::TileSet()
    : m_tileWidth(GameSettings::instance().getFieldTileWidth())
    , m_tileHeight(GameSettings::instance().getFieldTileHeight())
{
}

TileSet::~TileSet() {
    unload(); // 保持している画像ハンドルを全て開放
}

std::vector<TileDef> parseJson(const std::string& jsonPath) {
    std::ifstream ifs(jsonPath);
    json j;
    ifs >> j;

    std::vector<TileDef> result;

    // "frames"が存在するかチェック
    if (!j.contains("frames")) {
        printf("JSON error: 'frames' not found\n");
        return result;
    }

    for (auto& frame : j["frames"]) {

        // imageファイル名を取得
        if (!frame.contains("image")) {
            printf("JSON error: 'image' not found in frame\n");
            continue;
        }
        std::string imageFile = frame["image"];

        // "Tiles"が存在するかチェック
        if (!frame.contains("tiles")) {
            printf("JSON error: 'tiles' not found in frame\n");
            continue;
        }

        for (auto& item : frame["tiles"]) {
            TileDef def;
            def.image = imageFile;
            def.x = item["x"];
            def.y = item["y"];
            def.w = item["width"];
            def.h = item["height"];
            def.wall = item["wall"];
            result.push_back(def);
        }
    }
    return result;
}

bool TileSet::loadFromJson(const std::vector<std::string>& jsonPaths) {
    unload();

    for (const auto& jsonPath : jsonPaths) {

        // JSONファイルを解析してタイル定義を取得
        std::vector<TileDef> defs = parseJson(jsonPath);
        // defsが空なら次へ
        if (defs.empty()) { continue; }

        // jsonのあるディレクトリを基準に画像パスを生成
        std::string baseDir = jsonPath.substr(0, jsonPath.find_last_of("\\/") + 1);

        // imageごとにまとめる
        std::map<std::string, std::vector<TileDef>> grouped;

        for (auto& def : defs) {
            grouped[def.image].push_back(def);
        }

        for (auto& pair : grouped) {
            std::string imagePath = baseDir + pair.first;

            int tileset = LoadGraph(imagePath.c_str());
            if (tileset == -1) {
                printf("LoadGraph failed: %s\n", imagePath.c_str());
                unload();
                return false;
            }

            // タイル画像を切り出して登録
            for (const auto& d : pair.second) {
                int tile = DerivationGraph(d.x, d.y, d.w, d.h, tileset);
                if (tile == -1) {
                    unload();
                    DeleteGraph(tileset);
                    return false;
                }
                m_tileImages.push_back(tile);
                m_isWall.push_back(d.wall);
            }
            DeleteGraph(tileset);
        }
    }
    return true;
}

bool TileSet::isWall(int tileId) const {
    if (tileId < 0 || tileId >= m_isWall.size()) return false;
    return m_isWall[tileId];
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
