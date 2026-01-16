#define NOMINMAX
#include <limits>
#include "DxLib.h"
#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
	: m_keyHoldFrames{}  // キーのホールドフレームを0で初期化
{
}

void KeyboardInput::update() {
	std::array<char, KeyKindMax> keyState{};  // 各キーの状態を格納する配列
	
    // 全キーの状態を取得
	GetHitKeyStateAll(keyState.data());

	// 各キーのホールドフレームを更新する
	for (int i = 0; i < KeyKindMax; i++) {
		const bool pressed = (keyState[i] != 0);

		// キーが押されている場合、フレームを増加させる
		if (pressed) {
            if (m_keyHoldFrames[i] < std::numeric_limits<unsigned int>::max())  // オーバーフロー防止
                m_keyHoldFrames[i]++;
		}
		else {
			m_keyHoldFrames[i] = 0;
		}
	}
}

bool KeyboardInput::isTriggered(int keyIndex) const {
    // キーインデックスが無効の場合は押したと判断しない
	if (!isValidIndex(keyIndex)) {
		return false;
	}
	// キーが押された瞬間かどうか（前フレーム0 → 今フレーム1）
	return (m_keyHoldFrames[keyIndex] == 1);
}

int KeyboardInput::getHoldFrames(int keyIndex) const {
    // キーインデックスが無効の場合はフレーム数を-1にする
	if (!isValidIndex(keyIndex)) {
		return -1;
	}
    // キーが押され続けているフレーム数を返す
	return m_keyHoldFrames[keyIndex];
}

bool KeyboardInput::isValidIndex(int keyIndex) const {
    // キーインデックスが有効か確認する
	return (keyIndex >= 0 && keyIndex < KeyKindMax);
}
