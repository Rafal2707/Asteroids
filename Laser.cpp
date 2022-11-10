//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Laser.hpp"
#include "sre/Renderer.hpp"
#include "SpaceShip.hpp"
#include <chrono>
#include <thread>
#include "AsteroidsGame.hpp"
#include "Collidable.hpp"


Laser::Laser(const sre::Sprite &sprite) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    
    position = winSize * 0.5f;
    velocity = glm::vec2(0.0f,0.0f);

    direction = glm::rotateZ(glm::vec3(0, 0, 0), glm::radians(rotation));
    velocity += direction;
    float speed = glm::length(velocity);

    timeToLive = 1.0f;
}

void Laser::update(float deltaTime) {
    direction = glm::rotateZ(glm::vec3(0, 10.0f, 0), glm::radians(rotation));
    velocity += direction;
    float speed = glm::length(velocity);

    position += velocity * deltaTime;

    if (speed > maxSpeed) {
        velocity = velocity * (maxSpeed / speed);
    }
    // wrap around
    if (position.x < 0){
        position.x += winSize.x;
    } else if (position.x > winSize.x){
        position.x -= winSize.x;
    }
    if (position.y < 0){
        position.y += winSize.y;
    } else if (position.y > winSize.y){
        position.y -= winSize.y;
    }


    timeToLive -= deltaTime;

    if (timeToLive <= 0) {
        toBeDestructed = true;        
    }

}
 

void Laser::onCollision(std::shared_ptr<GameObject> other) {
    std::cout << "onCollision from Laser" << std::endl;

    //if (getPosition().x  == other->getPosition().x && getPosition().y + getRadius()) {
    //    toBeDestructed = true;
    //    other->toBeDestructed = true;
    //}
 
}



