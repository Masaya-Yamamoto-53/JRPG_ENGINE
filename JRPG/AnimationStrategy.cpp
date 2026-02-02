#include <fstream>
#include "DxLib.h"
#include "AnimationStrategy.h"

#include "json.hpp"
using json = nlohmann::json;

AnimationStrategy::AnimationStrategy()
    : m_frame(0)
    , m_animIndex(1)
{
}

void AnimationStrategy::loadImages(const std::string& baseDir, const std::string& id) {
    std::string jsonPath = baseDir + id + ".json";

    std::ifstream ifs(jsonPath);
    if (!ifs) {
        printf("JSON error: cannot open %s\n", jsonPath.c_str());
    }
    json j;
    ifs >> j;
    const auto& frameJson = j.at("frames").at(0);

    // jsonのあるディレクトリを基準に画像パスを生成
    auto pos = jsonPath.find_last_of("\\/");
    std::string imgDir = (pos == std::string::npos)
                        ? std::string()
                        : jsonPath.substr(0, pos + 1);

    std::string filePath = imgDir + frameJson.at("image").get<std::string>();

    // sprite
    const auto& spriteJson = frameJson.at("sprite");
    m_spriteWidth = spriteJson.at("width").get<int>();
    m_spriteHeight = spriteJson.at("height").get<int>();

    // images
    SetTransColor(0, 255, 0);  // 透過色を設定
    int handle = LoadGraph(filePath.c_str());

    int i = 0;
    for (const auto& imagesJson : frameJson.at("images")) {
        CharacterImage charImg;
        int x = imagesJson.at("x").get<int>();
        int y = imagesJson.at("y").get<int>();
        charImg.flip = imagesJson.at("flip").get<int>();
        charImg.handle = DerivationGraph(x, y, m_spriteWidth, m_spriteHeight, handle);

        std::string dir = "none";
        if (imagesJson.contains("Direction")) {
            dir = imagesJson.at("Direction").get<std::string>();
        }
        // インデックスを決定する
        if      (dir == "up"       ) { m_up = i;       }
        else if (dir == "down"     ) { m_down = i;     }
        else if (dir == "left"     ) { m_left = i;     }
        else if (dir == "right"    ) { m_right = i;    }
        else if (dir == "left-run" ) { m_leftRun = i;  }
        else if (dir == "right-run") { m_rightRun = i; }

        m_images.push_back(charImg);
        i++;
    }
    if (m_leftRun == -1) {
        m_leftRun = m_left;
    }
    if (m_rightRun == -1) {
        m_rightRun = m_right;
    }
}

const CharacterImage& AnimationStrategy::getImage() const {
    return m_images[m_animIndex];
}

void AnimationStrategy::update(Direction useDir, bool isMoving, bool running) {
    if(isMoving) {
        m_frame = (m_frame + 1) % imgPattern.size();
    }
    else {
        m_frame = 0;
    }
    if (useDir != Direction::None) {
        m_animIndex = calcAnimIndex(useDir, running);
        m_animIndex += imgPattern[m_frame];
    }
}

int AnimationStrategy::calcAnimIndex(Direction dir, bool running) const {
    int base = 0;
    switch (dir) {
    case Direction::Down:
        base = m_down;
        break;
    case Direction::Up:
        base = m_up;
        break;
    case Direction::Left:
        base = running ? m_leftRun  : m_left;
        break;
    case Direction::Right:
        base = running ? m_rightRun : m_right;
        break;
    default:
        break;
    }
    return base;
}
