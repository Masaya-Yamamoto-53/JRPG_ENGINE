#pragma once

class FpsController {
private:
    int m_targetFps;    // 目標フレームレート
    double m_fps;       // 実際のフレームレートを保持する
    double m_frameTime; // 1フレームかかった時間を保持する
    int m_startTime;    // 計測開始時刻を保持する
    int m_frameStart;   // 現在のフレーム開始時刻を保持する
    int m_frameCount;   // フレーム数をカウントする

public:
    FpsController(int targetFps);
    void update();         // フレームレートを更新する
    void wait();           // フレーム間隔を調整する
    double getFps() const; // 現在のフレームレートを取得する
};
