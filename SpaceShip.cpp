//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"
#include "AsteroidsGame.hpp"
#include "Asteroid.hpp"
#include "Laser.hpp"

using namespace std;

SpaceShip::SpaceShip(const sre::Sprite& sprite) : GameObject(sprite) {
    scale = glm::vec2(0.5f, 0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 23;
    position = winSize * 0.5f;
    velocity = glm::vec2(0.0f, 0.0f);
}



void SpaceShip::update(float deltaTime) {
        if (thrust) {
            float acceleration = deltaTime * thrustPower;
            glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));

            this->direction = direction;
            velocity += direction;
            float speed = glm::length(velocity);

            if (speed > maxSpeed) {
                velocity = velocity * (maxSpeed / speed);
            }
        }
        else {
            velocity = velocity * (1.0f - drag * deltaTime);
        }
        position += velocity * deltaTime;

        if (rotateCCW) {
            rotation += deltaTime * rotationSpeed;
        }
        if (rotateCW) {
            rotation -= deltaTime * rotationSpeed;
        }
    


    // wrap around
    if (position.x < 0) {
        position.x += winSize.x;
    }
    else if (position.x > winSize.x) {
        position.x -= winSize.x;
    }
    if (position.y < 0) {
        position.y += winSize.y;
    }
    else if (position.y > winSize.y) {
        position.y -= winSize.y;
    }
}

void SpaceShip::onCollision(std::shared_ptr<GameObject> other) {
    std::cout << "Collided with Spaceship" << std::endl;
}

void SpaceShip::onKey(SDL_Event& keyEvent) {
    if (game->isPlaying) {
        if (keyEvent.key.keysym.sym == SDLK_UP) {
            thrust = keyEvent.type == SDL_KEYDOWN;
        }
        if (keyEvent.key.keysym.sym == SDLK_LEFT) {
            rotateCCW = keyEvent.type == SDL_KEYDOWN;
        }
        if (keyEvent.key.keysym.sym == SDLK_RIGHT) {
            rotateCW = keyEvent.type == SDL_KEYDOWN;
        }
        if (keyEvent.type == SDL_KEYDOWN && keyEvent.key.keysym.sym == SDLK_SPACE) {

            game->gameObjects.push_back(std::make_shared<Laser>(laserSprite));

            std::shared_ptr<GameObject> spaceShip = game->gameObjects[0];
            std::shared_ptr<GameObject> lastLaser = game->gameObjects[game->gameObjects.size() - 1];

            glm::vec2 spaceShipDirection = spaceShip->getDirection();
            glm::vec2 lastLaserDirection = lastLaser->getDirection();

            lastLaser->setPosition(this->position);
            lastLaser->setRotation(this->rotation);
        }
    }
}