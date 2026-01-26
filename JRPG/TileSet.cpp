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
    // 保持している画像ハンドルを全て開放
    unload();
}

std::vector<TileFrame> TileSet::loadJsonDefs(const std::string& jsonPath) const {
    std::vector<TileFrame> frames;

    // JSONファイルを開く
    std::ifstream ifs(jsonPath);
    if (!ifs) {
        printf("JSON error: cannot open %s\n", jsonPath.c_str());
        return frames;
    }

    json j;
    try {
        ifs >> j;
    }
    catch (const std::exception& e) {
        printf("JSON parse error in %s: %s\n", jsonPath.c_str(), e.what());
        return frames;
    }

    // "frames"が存在するかチェック
    if (!j.contains("frames") || !j["frames"].is_array()) {
        printf("JSON error: 'frames' not found or is not an array in %s\n", jsonPath.c_str());
        return frames;
    }

    // 各frameを読み込む
    for (const auto& f : j["frames"]) {

        // imageが存在するか
        if (!f.contains("image") || !f["image"].is_string()) {
            printf("JSON error: 'image' not found or is not a string in frame\n");
            continue;
        }

        TileFrame frameDef;
        frameDef.image = f["image"].get<std::string>();

        // タイル定義を読み込む
        if (!f.contains("tiles") || !f["tiles"].is_array()) {
            printf("JSON error: 'tiles' not found or is not an array in frame\n");
            continue;
        }

        // タイル定義を読み込む
        for (const auto& item : f["tiles"]) {
            TileDef def;

            def.x = item.value("x", 0);
            def.y = item.value("y", 0);
            def.w = item.value("width", 0);
            def.h = item.value("height", 0);
            def.wall = item.value("wall", false);

            frameDef.frame.push_back(def);
        }
        frames.push_back(frameDef);
    }
    printf("Debug: Loaded %2zu frame definitions from %s\n", frames.size(), jsonPath.c_str());
    return frames;
}


bool TileSet::loadTileImages(
      const std::vector<TileFrame>& defs
    , const std::string& baseDir
    , TileSetEntry& outEntry
) {
    outEntry.layers.clear();

    if (defs.empty()) {
        return true;
    }

    // 最初のフレームのタイル数を基準にする
    int expectedTileCount = defs[0].frame.size();

    for (size_t i = 0; i < defs.size(); i++) {
        const auto& def = defs[i];

        // 整合性チェック
        if (def.frame.size() != expectedTileCount) {
            printf("Tile count mismatch in frame %zu: expected %d, got %zu\n"
                , i, expectedTileCount, def.frame.size());
            return false;
        }

        // 画像パスを生成
        std::string imagePath = baseDir + def.image;

        // 画像を読み込む
        int tileset = LoadGraph(imagePath.c_str());
        if (tileset == -1) {
            printf("LoadGraph failed: %s\n", imagePath.c_str());
            return false;
        }

        TileLayer layer;

        // 各タイルを切り出して登録
        for (const auto& d : def.frame) {
            int tile = DerivationGraph(d.x, d.y, d.w, d.h, tileset);
            if (tile == -1) {
                for (int h : layer.images) {
                    if (h != -1) {
                        DeleteGraph(h);
                    }
                }
                DeleteGraph(tileset);
                return false;
            }
            layer.images.push_back(tile);
            layer.isWall.push_back(d.wall);
        }

        // 読み込み後、元の画像は不要なので開放
        DeleteGraph(tileset);

        // フレーム単位で追加
        outEntry.layers.push_back(layer);
    }

    return true;
}

void TileSet::addEntry(const TileSetEntry &entry) {
    m_entries.push_back(entry);
}

bool TileSet::loadFromJson(const std::vector<std::string>& jsonPaths) {
    // 既存データを開放
    unload();

    for (const auto& jsonPath : jsonPaths) {

        // JSONファイルを解析してタイル定義を取得
        std::vector<TileFrame> defs = loadJsonDefs(jsonPath);
        // defsが空なら次へ
        if (defs.empty()) { continue; }

        // jsonのあるディレクトリを基準に画像パスを生成
        auto pos = jsonPath.find_last_of("\\/");
        std::string baseDir = (pos == std::string::npos)
                            ? std::string()
                            : jsonPath.substr(0, pos + 1);

        // フレームごとにタイル画像を読み込み
        TileSetEntry entry;

        // タイル画像の読み込み
        if (!loadTileImages(defs, baseDir, entry)) {
            printf("Tileset load failed for %s - skipping\n", jsonPath.c_str());
            continue;
        }
        // タイルセットに登録
        addEntry(entry);
    }

    return true;
}

bool TileSet::isWall(int entryId, int tileId, int frameId) const {
    if (entryId < 0 || entryId >= static_cast<int>(m_entries.size())) {
        return false;
    }
    const auto& entry = m_entries[entryId];
    if(entry.layers.empty()) {
        return false;
    }
    const int frameMax = static_cast<int>(entry.layers.size());
    // フレームIDを有効範囲に収める
    int frameIndex = ((frameId % frameMax) + frameMax) % frameMax;

    const auto& layer = entry.layers[frameIndex];
    if (tileId < 0 || tileId >= static_cast<int>(layer.isWall.size())) {
        return false;
    }
    return layer.isWall[tileId];
}

void TileSet::unload() {
    for (auto& entry : m_entries) {
        for (auto& layer : entry.layers) {
            for (int& handle : layer.images) {
                if (handle != -1) {
                    // 保持している画像ハンドルを開放
                    DeleteGraph(handle);
                }
            }
            layer.images.clear();
            layer.isWall.clear();
        }
        entry.layers.clear();
    }
    m_entries.clear();
    m_entries.shrink_to_fit();
}

int TileSet::getTileImage(int entryId, int tileId, int counter) const
{
    if (entryId < 0 || entryId >= static_cast<int>(m_entries.size())) {
        return -1;
    }
    const auto& entry = m_entries[entryId];
    if (entry.layers.empty()) {
        return -1;
    }

    const auto& baseLayer = entry.layers[0];
    if (tileId < 0 || tileId >= static_cast<int>(baseLayer.images.size())) {
        return -1;
    }

    const int frameCount = static_cast<int>(entry.layers.size());
    if (frameCount <= 0) {
        return -1;
    }

    int frameIndex = counter % frameCount;
    const auto& layer = entry.layers[frameIndex];
    return layer.images[tileId];
}


