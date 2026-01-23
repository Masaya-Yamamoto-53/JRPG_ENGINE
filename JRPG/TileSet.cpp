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

std::vector<TileFrame> parseJson(const std::string& jsonPath) {
    std::ifstream ifs(jsonPath);
    json j;
    ifs >> j;

    std::vector<TileFrame> frame;

    // "frames"が存在するかチェック
    if (!j.contains("frames")) {
        printf("JSON error: 'frames' not found\n");
        return frame;
    }

    for (auto& f : j["frames"]) {

        // imageファイル名を取得
        if (!f.contains("image")) {
            printf("JSON error: 'image' not found in frame\n");
            continue;
        }
        TileFrame frameDef;
        frameDef.image = f["image"];

        // "Tiles"が存在するかチェック
        if (!f.contains("tiles")) {
            printf("JSON error: 'tiles' not found in frame\n");
            continue;
        }

        for (auto& item : f["tiles"]) {
            TileDef def;
            def.x = item["x"];
            def.y = item["y"];
            def.w = item["width"];
            def.h = item["height"];
            def.wall = item["wall"];
            frameDef.frame.push_back(def);
        }
        frame.push_back(frameDef);
    }
    return frame;
}

bool TileSet::loadFromJson(const std::vector<std::string>& jsonPaths) {
    unload();

    for (const auto& jsonPath : jsonPaths) {

        // JSONファイルを解析してタイル定義を取得
        std::vector<TileFrame> defs = parseJson(jsonPath);
        // defsが空なら次へ
        if (defs.empty()) { continue; }

        // jsonのあるディレクトリを基準に画像パスを生成
        std::string baseDir = jsonPath.substr(0, jsonPath.find_last_of("\\/") + 1);

        for (auto& def : defs) {
            std::string imagePath = baseDir + def.image;

            int tileset = LoadGraph(imagePath.c_str());
            if (tileset == -1) {
                printf("LoadGraph failed: %s\n", imagePath.c_str());
                unload();
                return false;
            }

            // タイル画像を切り出して登録
            for (const auto& d : def.frame) {
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
