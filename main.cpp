#include "Game.h"

int main() {

    while (true) {
        Game game;
        if (!game.gameProcess()) {
            break;
        }
    }
    return 0;
}