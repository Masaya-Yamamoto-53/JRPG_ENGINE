#pragma once

class GameScene {
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~GameScene() = default;
};
