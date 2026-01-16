#pragma once

class DebugManager {
private:
    bool m_enabled;

public:
    static DebugManager& instance();

    void toggle();
    bool enabled() const;

private:
    DebugManager();
};
