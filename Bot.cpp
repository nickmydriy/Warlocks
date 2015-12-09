#include "Bot.h"

Bot::Bot(GameWindow *gameWindow, Warlock *warlock, int randomKey) {
    this->gameWindow = gameWindow;
    this->warlock = warlock;
    time = clock();
    srand(randomKey);
}

void Bot::botStep(wsm::vec2 playerPosition, float magmaRadius) {
    if (time < clock()) {
        time = clock() + 500;
        float angle = (double)rand() / (double)RAND_MAX * 6.28;
        float radius = (double)rand() / (double)RAND_MAX * magmaRadius;
        warlock->targetPosition = wsm::vec2(std::cos(angle) * radius, std::sin(angle) * radius);
    } else {
        if (warlock->position == warlock->targetPosition) {
            time = clock() + 500;
            float angle = (double)rand() / (double)RAND_MAX * 6.28;
            float radius = (double)rand() / (double)RAND_MAX * magmaRadius;
            warlock->targetPosition = wsm::vec2(std::cos(angle) * radius, std::sin(angle) * radius);
        }
    }
    if (warlock->isFire) {
        if (wsm::distance(warlock->position, warlock->fireBallPosition) > magmaRadius * 1.5) {
            warlock->setFire(playerPosition);
        }
    } else {
        warlock->setFire(playerPosition);
    }
}