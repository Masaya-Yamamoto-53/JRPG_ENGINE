#include "DebugManager.h"

DebugManager& DebugManager::instance() {
    static DebugManager instance;
    return instance;
}

DebugManager::DebugManager()
    : m_enabled(false)
{
}

void DebugManager::toggle() {
    m_enabled = !m_enabled;
}

bool DebugManager::enabled() const {
    return m_enabled;
}
