//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Asteroid.hpp"
#include "Laser.hpp"



using namespace sre;

AsteroidsGame::AsteroidsGame() {
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
        .withSdlWindowFlags(SDL_WINDOW_OPENGL);
    time_t t;                                               // random seed based on time
    srand((unsigned)time(&t));
    atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

    initiateObjects();
    isPlaying = true;
    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event& event) {
        keyEvent(event);
    };

    r.frameRender = [&]() {
        render();
    };

    r.startEventLoop();
}


void AsteroidsGame::update(float deltaTime) {

    for (int i = 0; i < gameObjects.size(); i++) {
        if (isPlaying) {
            gameObjects[i]->update(deltaTime);
            checkColissions(gameObjects[i], i);

            //checks if the objects are removable, if yes, then remove them
            removeObjects();
        }

    }
}

void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius){
    float quaterPi = glm::quarter_pi<float>();
    for (float f = 0;f<glm::two_pi<float>();f += quaterPi){
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f)*radius,
                                  position.y + sinf(f)*radius,
                                  0
        });
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f+quaterPi)*radius,
                                  position.y + sinf(f+quaterPi)*radius,
                                  0
        });
    }
}

void AsteroidsGame::render() {
    auto renderPass = RenderPass::create()
            .withCamera(camera)
            .withClearColor(true, {.20, .60, .86, 1})
            .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles){
        std::vector<glm::vec3> lines;
        for (auto & go : gameObjects){
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr){
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x/2-100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i",score);
    ImGui::End();
}



void AsteroidsGame::checkColissions(std::shared_ptr<GameObject> gameObject, int index) {
    for (int j = index + 1; j < gameObjects.size(); j++) {
        //count the distance between objects
        float diffY = gameObjects[index]->position.y - gameObjects[j]->position.y;
        float diffX = gameObjects[index]->position.x - gameObjects[j]->position.x;
        float distanceBetweenObjects = sqrt((diffY * diffY) + (diffX * diffX));

        //SpaceShip hits asteroid(s)
        if (distanceBetweenObjects <= std::dynamic_pointer_cast<Collidable>(gameObjects[index])->getRadius() + std::dynamic_pointer_cast<Collidable>(gameObjects[j])->getRadius() &&
            std::dynamic_pointer_cast<SpaceShip>(gameObjects[index]) != nullptr && std::dynamic_pointer_cast<Asteroid>(gameObjects[j]) != nullptr) {
            auto bang = atlas->get("bang.png");
            gameObjects[index]->sprite = bang;
            isPlaying = false;
        }
        //Laser hits asteroids(s)
        if (distanceBetweenObjects <= std::dynamic_pointer_cast<Collidable>(gameObjects[index])->getRadius() + std::dynamic_pointer_cast<Collidable>(gameObjects[j])->getRadius() &&
            std::dynamic_pointer_cast<Asteroid>(gameObjects[index]) != nullptr && std::dynamic_pointer_cast<Laser>(gameObjects[j]) != nullptr) {

            auto asteroidB = atlas->get("Meteors/meteorBrown_big1.png");
            auto asteroidM = atlas->get("Meteors/meteorBrown_med1.png");
            auto asteroidS = atlas->get("Meteors/meteorBrown_small1.png");

            //check if the asterodid was big and spall medium ones
            if (gameObjects[index]->sprite.getSpriteSize() == asteroidB.getSpriteSize()) {
                for (int x = 0; x < 2; x++) {
                    gameObjects.push_back(std::make_shared<Asteroid>(asteroidM));
                    gameObjects[gameObjects.size() - 1]->setPosition(gameObjects[index]->getPosition());
                    std::dynamic_pointer_cast<Collidable>(gameObjects[gameObjects.size() - 1])->setRadius(10);
                }
            }
            //check if the asteroid was medium and spawn small ones
            else if (gameObjects[index]->sprite.getSpriteSize() == asteroidM.getSpriteSize()) {
                gameObjects.push_back(std::make_shared<Asteroid>(asteroidS));
                gameObjects[gameObjects.size() - 1]->setPosition(gameObjects[index]->getPosition());
                std::dynamic_pointer_cast<Collidable>(gameObjects[gameObjects.size() - 1])->setRadius(6);
            }
            //removing laser and asteroid that was hit 
            gameObjects[index]->toBeDestructed = true;
            gameObjects[j]->toBeDestructed = true;
        }
    }
}

void AsteroidsGame::removeObjects() {
    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [](std::shared_ptr<GameObject> go) {
        return go->toBeDestructed;
        }),
        gameObjects.end());
}

void AsteroidsGame::keyEvent(SDL_Event &event) {
    for (int i = 0; i < gameObjects.size();i++) {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
        debugCollisionCircles = !debugCollisionCircles;
    }
    if (event.type = SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !isPlaying) {
        restart();
    }
}

void AsteroidsGame::initiateObjects() {
    auto spaceshipSprite = atlas->get("playerShip1_blue.png");
    auto laserSprite = atlas->get("Lasers/laserBlue01.png");
    auto asteroidB = atlas->get("Meteors/meteorBrown_big1.png");

    auto spaceShip = std::make_shared<SpaceShip>(spaceshipSprite);
    auto laser = std::make_shared<Laser>(laserSprite);
    spaceShip->game = this;
    spaceShip->laserSprite = laserSprite;
    gameObjects.push_back(spaceShip);

    //Spawn 5 asteroids at the beginning
    for (int i = 0; i < 5; i++) {
        gameObjects.push_back(std::make_shared<Asteroid>(asteroidB));
    }
}

void AsteroidsGame::restart() {
    gameObjects.clear();
    initiateObjects();
    isPlaying = true;
}
int main(){
    AsteroidsGame* game = new AsteroidsGame();
    
    return 0;
}
   