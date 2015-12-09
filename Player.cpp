#include "Player.h"

Player::Player(GameWindow *gameWindow, Warlock *warlock) {
    this->gameWindow = gameWindow;
    gameWindow->addKeyListener(GLFW_KEY_SPACE);
    this->warlock = warlock;
}

void Player::playerStep(double screenMoveSpeed) {
    double xClick, yClick;
    if (gameWindow->mouseHandler(xClick, yClick, screenMoveSpeed)) {
        warlock->targetPosition = wsm::vec2(xClick, yClick);
    }
    if(gameWindow->isKeyClicked(GLFW_KEY_SPACE, xClick, yClick)) {
        warlock->setFire(wsm::vec2(xClick, yClick));
    }
}
