#pragma once
#include <vector>
#include <string>

class Party {
private:
    std::vector<int> m_memberIds;

public:
    // パーティにメンバーを追加
    void addMember(int memberId);

    // パーティからメンバーを削除
    void removeMember(int memberId);

};

