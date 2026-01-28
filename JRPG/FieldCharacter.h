#pragma once
#include <string>
#include <vector>
#include <memory>
#include "common.h"
#include "IFieldEntity.h"
#include "ICharacterAnimation.h"

class FieldCharacter : public IFieldEntity {
private:
    std::string m_id;       // キャラクタID
    std::string m_baseDir;  // ベースディレクトリ

    int m_x;  // 現在X座標
    int m_y;  // 現在Y座標

    Direction m_direction; //現在向いている方向

    std::unique_ptr<ICharacterAnimation> m_anim;  // アニメーション制御オブジェクト

public:
    FieldCharacter(
          std::string id
        , std::string baseDir
        , std::unique_ptr<ICharacterAnimation> anim
    );

    void loadImage(std::string baseDir, std::string id);

    // キャラクタの状態更新
    void update(
          const MoveAmounts & amounts
        , Direction direction);

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

};
