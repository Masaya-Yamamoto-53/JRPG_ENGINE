#pragma once
#include <string>

class GameSettings {
private:
    int m_targetFps = 60;
    bool m_windowMode = true;

    int m_spriteWidth  = 64;
    int m_spriteHeight = 64;

    int m_fieldTileWidth = 32;
    int m_fieldTileHeight = 32;

    int m_tileCountX = 25;
    int m_tileCountY = 16;

    int m_windowWidth;
    int m_windowHeight;
    int m_colorBitDepth = 32;

public:
    // シングルトンとして利用するための取得関数
    static GameSettings& instance();

    // 設定ファイルを読み込む
    bool load(const std::string& filePath);

    // 目標FPSを取得
    int getTargetFps() const { return m_targetFps; }
    // ウィンドウモードを取得
    bool isWindowMode() const { return m_windowMode; }

    // スプライト幅（ピクセル単位）を取得する
    int getSpriteWidth() const { return m_spriteWidth; }
    // スプライト高さ（ピクセル単位）を取得する
    int getSpriteHeight() const { return m_spriteHeight; }

    // フィールドタイル幅（ピクセル単位）を取得する
    int getFieldTileWidth() const { return m_fieldTileWidth; }
    // フィールドタイル高さ（ピクセル単位）を取得する
    int getFieldTileHeight() const { return m_fieldTileHeight; }

    // 横方向のタイル数
    int getTileCountX() const { return m_tileCountX; }
    // 縦方向のタイル数
    int getTileCountY() const { return m_tileCountY; }

    // ウィンドウの幅（ピクセル単位）を取得
    int getWindowWidth() const { return m_windowWidth; }
    // ウィンドウの高さ（ピクセル単位）を取得する
    int getWindowHeight() const { return m_windowHeight; }
    // カラービット深度を取得する
    int getColorBitDepth() const { return m_colorBitDepth; }
};
