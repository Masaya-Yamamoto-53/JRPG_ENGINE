#include "Party.h"

// パーティにメンバーを追加
void Party::addMember(int memberId) {
    m_memberIds.push_back(memberId);
}

// パーティからメンバーを削除
void Party::removeMember(int memberId) {
    m_memberIds.erase(
          std::remove(m_memberIds.begin(), m_memberIds.end(), memberId)
        , m_memberIds.end()
    );
}
