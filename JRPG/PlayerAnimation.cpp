#include <fstream>
#include "DxLib.h"
#include "PlayerAnimation.h"

#include "json.hpp"
using json = nlohmann::json;

PlayerAnimation::PlayerAnimation()
    : m_running(false)
    , m_frame(0)
    , m_runCounter(0)
    , m_animIndex(1)
{
}

void PlayerAnimation::loadImages(const std::string& baseDir, const std::string& id) {
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
        charImg.handleUpper = DerivationGraph(x, y                           , m_spriteWidth, (m_spriteHeight / 4) * 3, handle);
        charImg.handleLower = DerivationGraph(x, y + (m_spriteHeight / 4) * 3, m_spriteWidth, (m_spriteHeight / 4)    , handle);

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
}

const CharacterImage& PlayerAnimation::getImage() const {
    return m_images[m_animIndex];
}

int PlayerAnimation::getMoveAmount() const {
    return (m_running ? RunSpeed : WalkSpeed);
}

void PlayerAnimation::updateAnimation(Direction useDir, bool isMoving) {
    if(isMoving) {
        m_runCounter = (std::min)(m_runCounter + 1, RunStartFrame);
        m_running = (m_runCounter >= RunStartFrame);
        m_frame = (m_frame + 1) % imgPattern.size();
    }
    else {
        m_runCounter = 0;
        m_running = false;
        m_frame = 0;
    }
    if (useDir != Direction::None) {
        m_animIndex = calcAnimIndex(useDir);
    }
}

int PlayerAnimation::calcAnimIndex(Direction dir) const {
    int base = 0;
    switch (dir) {
    case Direction::Down:
        base = m_down;
        break;
    case Direction::Up:
        base = m_up;
        break;
    case Direction::Left:
        base = m_running ? m_leftRun  : m_left;
        break;
    case Direction::Right:
        base = m_running ? m_rightRun : m_right;
        break;
    default:
        break;
    }
    return base + imgPattern[m_frame];
}

int PlayerAnimation::getSpriteWidth() const { return m_spriteWidth; }
int PlayerAnimation::getSpriteHeight() const { return m_spriteHeight; }
