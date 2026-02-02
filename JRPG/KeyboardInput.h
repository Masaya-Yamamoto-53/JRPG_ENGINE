#pragma once
#include <array>
#include "DxLib.h"

class KeyboardInput {
private:
    // DXライブラリで扱えるキーの総数（0～255）
    static constexpr int KeyKindMax = 256;

    // 各キーが「何フレーム連続でおされているか」を記録する配列
    // インデックスはキーコードに対応
    std::array<int, KeyKindMax> m_keyHoldFrames;
public:
    // コンストラクタ：ホールドフレームをすべて0で初期化
    KeyboardInput();
    // 参フレーム呼び出し、前期―の押下状態を取得してホールドフレームを更新する
    void update();
    // 指定したキーが「押された瞬間（前フレーム0 -> 今フレーム1）」かどうかを判定する
    bool isTriggered(int keyIndex) const;
    // 指定したキーが「押され続けている」フレーム数を返す
    // 押されていない場合は 0
    // 無効なキーインデックスの場合は -1 を返す
    int getHoldFrames(int keyIndex) const;

private:
    // キーインデックスが有効範囲（0～KeyKindMax-1）に収まっているかを確認
    bool isValidIndex(int keyIndex) const;
};
