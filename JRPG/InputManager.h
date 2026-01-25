#pragma once
#include "KeyboardInput.h"

struct DirectionalHoldFrames {
    int up;
    int down;
    int left;
    int right;
};

class InputManager {
private:
    // キーボード入力管理クラス
    KeyboardInput m_keyboard;

public:
    // シングルトンとして利用するための取得関数
    static InputManager& instance();
    // 毎フレーム呼び出し、全ての入力デバイスを更新する
    void update();
    // 指定したキーが押された瞬間かどうかを判定
    bool isKeyTriggered(int keyIndex) const;
    // 指定したキーの入力継続時間を取得
    int getKeyHoldFrames(int keyIndex) const;
    // 方向キーの入力継続時間を取得
    DirectionalHoldFrames getDirectionalHoldFrames();

private:
    //コンストラクタをprivateにしてシングルトン化
    InputManager();

};
