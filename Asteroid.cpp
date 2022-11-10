//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Asteroid.hpp"
#include "sre/Renderer.hpp"
#include <random>

Asteroid::Asteroid(const sre::Sprite &sprite) : GameObject(sprite) {
    scale = glm::vec2(0.5f,0.5f);
    float r = (rand() % 10 + 1)/10.0f;

    //avoid spawning on spaceship at the beginning
    if (r == 0.5f) {
        r = (rand() % 10 + 6) / 10.0f;
    }
    winSize = sre::Renderer::instance->getDrawableSize();
    std::cout << r << std::endl;
    position = winSize * r;
    velocity = glm::vec2(rand() % 50 + 10, rand() % 50 + 10);
    rotationSpeed = rand() % 100 + 10;
    radius = 24;
    std::cout << radius << std::endl;
}
void Asteroid::onCollision(std::shared_ptr<GameObject> other)
{
    std::cout << "Collided with Asteroid" << std::endl;
}
void Asteroid::update(float deltaTime) {
    
    position += velocity * deltaTime;
    rotation += deltaTime * rotationSpeed ;

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
}





