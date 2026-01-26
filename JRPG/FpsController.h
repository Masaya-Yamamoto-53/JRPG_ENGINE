#pragma once
#include <vector>

class FpsController {
private:
    int m_targetFps;      // 目標フレームレート
    double m_fps;         // 実際のフレームレートを保持する
    double m_frameTime;   // 1フレームかかった時間を保持する
    double m_startTime;   // 計測開始時刻を保持する
    double m_frameStart;  // 現在のフレーム開始時刻を保持する
    int m_frameCount;     // フレーム数をカウントする

    long long m_qpcStart;
    long long m_qpcFreq;

    // デバッグ情報用の履歴
    std::vector<double> m_frameTimeHistory; // 過去フレームの処理時間
    int m_frameTimeHistoryIndex;            // 履歴のインデックス
    double m_frameTimeHistorySum;           // 履歴の合計値

    std::vector<double> m_slackTimeHistory; // 過去フレームの余裕時間
    int m_slackTimeHistoryIndex;            // 余裕時間履歴のインデックス
    double m_slackTimeHistorySum;           // 余裕時間履歴の合計値

public:
    static FpsController& instance();

    void update();  // フレームレートを更新する
    void wait();    // フレーム間隔を調整する

    // 現在のフレームレートを取得する
    double getFps() const { return m_fps; }

    // デバッグ情報取得
    double getFrameTimeMin()     const { return calcMin(m_frameTimeHistory);    }
    double getFrameTimeMax()     const { return calcMax(m_frameTimeHistory);    }
    double getFrameTimeMedian()  const { return calcMedian(m_frameTimeHistory); }
    double getFrameSlackMin()    const { return calcMin(m_slackTimeHistory);    }
    double getFrameSlackMax()    const { return calcMax(m_slackTimeHistory);    }
    double getFrameSlackMedian() const { return calcMedian(m_slackTimeHistory); }

private:
    FpsController(int targetFps);

    // 計算用関数
    inline double getTimeUS();
    double calcMin(const std::vector<double>& data) const;
    double calcMax(const std::vector<double>& data) const;
    double calcMedian(std::vector<double> data) const;
};
