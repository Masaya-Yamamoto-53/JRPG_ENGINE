#pragma once
#include "KeyboardInput.h"

class InputManager {
private:
    // キーボード入力管理クラス
    KeyboardInput m_keyboard;

public:
    // シングルトンとして利用するための取得関数
    static InputManager& instance();

    // 毎フレーム呼び出し、全ての入力デバイスを更新する
    void update();

    bool isKeyTriggered(int keyIndex) const;

private:
    //コンストラクタをprivateにしてシングルトン化
    InputManager();
};
