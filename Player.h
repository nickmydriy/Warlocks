#pragma once
#include "Warlock.h"
#include "GameWindow.h"

/**
 * Contains player interface to control warlock
 */
class Player {
public:
    /**
     * Constuct Player interface
     * \param gameWindow Window to draw warlock model
     * \param warlock Warlock to control it
     */
    Player(GameWindow *gameWindow, Warlock *warlock);
    /**
     * Handle mouse and keyboard
     * Then do some changes on warlock
     * \param screenMoveSpeed Points to move camera (in GL points)
     */
    void playerStep(double screenMoveSpeed);
private:
    GameWindow *gameWindow;
    Warlock *warlock;
};