#pragma once

enum class Direction {
      None
    , Up
    , Down
    , Left
    , Right
};

struct MoveAmounts {
    int up;
    int down;
    int left;
    int right;
    bool upFlag;
    bool downFlag;
    bool leftFlag;
    bool rightFlag;
};

