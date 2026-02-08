#include "TitleScene.h"
#include <fstream>
#include "SceneManager.h"
#include "InputDeviceHub.h"
#include "DxLib.h"

#include "json.hpp"
using json = nlohmann::json;

TitleScene::TitleScene(SceneManager* sceneManager)
    : m_sceneManager(sceneManager)
    , m_selectedIndex(0)
{
}

void TitleScene::update() {
    int numItems = m_menuItems.size();

    if (InputDeviceHub::instance().isKeyTriggered(InputDeviceHub::KEY_UP)) {
        m_selectedIndex = (m_selectedIndex - 1 + numItems) % numItems;
    }
    if (InputDeviceHub::instance().isKeyTriggered(InputDeviceHub::KEY_DOWN)) {
        m_selectedIndex = (m_selectedIndex + 1) % numItems;
    }
    if (InputDeviceHub::instance().isKeyTriggered(InputDeviceHub::KEY_ENTER)) {
        m_sceneManager->changeScene(SceneType::Field);
    }
}

void TitleScene::render() {
    for (int i = 0; i < m_menuItems.size(); i++) {
        const MenuItem& item = m_menuItems[i];
        bool selected = (i == m_selectedIndex);

        /*

        // 画像が存在する場合（normal と selected の両方が有効）
        if (item.normalHandle != -1 && item.selectedHandle != -1) {
            int handle = selected ? item.selectedHandle : item.normalHandle;
            DrawGraph(item.x, item.y, handle, TRUE);
        }
        else {
            // 画像がない場合 → label をテキストで描画
            int color = selected ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
            const char* prefix = selected ? "> " : "  ";
            DrawString(item.x, item.y, (std::string(prefix) + item.label).c_str(), color);
        }
        */
    }
}


void TitleScene::load() {
    // JSONファイルを開く
    std::string jsonPath = "asets//title_menu.json";
    std::ifstream ifs(jsonPath);
    if (!ifs) {
        printf("JSON error: cannot open %s\n", jsonPath.c_str());
    }
    json j;
    try {
        ifs >> j;
    }
    catch (const std::exception& e) {
        printf("JSON parse error in %s: %s\n", jsonPath.c_str(), e.what());
    }

    for (auto& item : j["menu"]) {
        MenuItem m;
        m.label = item["label"].get<std::string>();
        m.x = item["x"].get<int>();
        m.y = item["y"].get<int>();
        m.action = item["action"].get<std::string>();

        // 画像読み込み
        std::string normalPath = item["normal_image"].get<std::string>();
        std::string selectedPath = item["selected_image"].get<std::string>();

        // 読み込み失敗時は-1
        m.normalHandle = LoadGraph(normalPath.c_str());
        m.selectedHandle = LoadGraph(selectedPath.c_str());

        m_menuItems.push_back(m);
    }
}

