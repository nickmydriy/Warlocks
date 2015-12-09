#pragma once
#include "Warlock.h"
#include "GameWindow.h"

class Bot {
public:
    /**
     * Create bot for warlocks game
     * \param gameWindow Window to draw warlock model
     * \param warlock Warlock to control
     * \param randomKey Any number. Needs to randomize bot logic
     */
    Bot(GameWindow *gameWindow, Warlock *warlock, int randomKey);
    /**
     * Solutions to do
     * \param playerPosition Position of player.
     * \param magmaRadius Radius of magma in game
     */
    void botStep(wsm::vec2 playerPosition, float magmaRadius);
private:
    GameWindow *gameWindow;
    Warlock *warlock;
    float time;
};
