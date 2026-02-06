#pragma once
#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "AnimationStrategy.h"
#include "MovementStrategy.h"

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

class FieldCharacter {
private:
    int m_x;  // 現在X座標
    int m_y;  // 現在Y座標

    std::unique_ptr<MovementStrategy> m_move;
    std::unique_ptr<AnimationStrategy> m_anim;  // アニメーション制御オブジェクト

public:
    FieldCharacter(
          std::string id
        , std::string baseDir
        , std::unique_ptr<MovementStrategy> move
        , std::unique_ptr<AnimationStrategy> anim
        , int x
        , int y
    );

    void loadImage(std::string baseDir, std::string id);

    // キャラクタの状態更新
    void update(const MoveAmounts& amounts, Direction direction);
    // 現在のX座標を取得する
    int getX() const;
    // 現在のY座標を取得する
    int getY() const;
    // 現在の移動量（アニメーション用）を取得する
    int getMoveAmount() const;
    // スプライト画像の幅
    int getSpriteWidth() const;
    // スプライト画像の高さ
    int getSpriteHeight() const;
    // 現在のアニメーションで使用する画像IDを取得する
    const CharacterImage& getImage() const;

    Rect getHitBox() const {
        return { m_x, m_y, getSpriteWidth(), getSpriteHeight() };
    }
};
