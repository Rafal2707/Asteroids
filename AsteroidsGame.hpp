#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

class GameObject;


class AsteroidsGame {
public:
    AsteroidsGame();

    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    bool isPlaying;

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event& event);
    void checkColissions(std::shared_ptr<GameObject> gameObject, int index);
    void removeObjects();
    void restart();
    void initiateObjects();
    sre::Camera camera;
    sre::SDLRenderer r;

    bool debugCollisionCircles = false;
    int score = 0;
};


