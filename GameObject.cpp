//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#include "GameObject.hpp"
#include "AsteroidsGame.hpp"

GameObject::GameObject(sre::Sprite sprite)
    :position(0, 0),
    rotation(0),
    scale(1, 1),
    sprite(sprite),
    direction(0, 0),
    toBeDestructed(false)
    
{
}


GameObject::~GameObject() {
    std::cout << "Calling destructor" << std::endl;
}


void GameObject::setPosition(glm::vec2 newPosition) {
   position = newPosition;
}


void GameObject::update(float deltaTime) {
}

void GameObject::render(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {
    sprite.setPosition(position);
    sprite.setRotation(rotation);
    sprite.setScale(scale);
    spriteBatchBuilder.addSprite(sprite);
}

void GameObject::onKey(SDL_Event& keyEvent) {

}

glm::vec2 GameObject::getPosition() {
    return position;
}

void GameObject::setRotation(float newRotation) {
    rotation = newRotation;
}

glm::vec2 GameObject::getDirection() {
    return direction;
}

void GameObject::setDirection(glm::vec2 newDirection) {
    direction = newDirection;
}