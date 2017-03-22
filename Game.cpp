#include "Game.h"

// TODO:
#define TIME_DIVISOR 100000

Game::Game() {
    //get settings
    std::vector<int> settings = getSettings();
    fullScreen = settings[3];
    //open window
    gameWindow = new GameWindow(settings[0], settings[1], 20, 80, "warlocks", fullScreen);
    //initialize models
    warlockModel = new Model(gameWindow, "Objects/warlock.obj", "Textures/warlock.dds");
    warlockModel->setSimpleRingAnimation(0.1, 1, 0.2, 0.15, 0.25, 1);
    magmaModel = new Model(gameWindow, "Objects/magma.obj", "Textures/magma.dds");
    magmaModel->setSimpleRingAnimation(0.1, 40, 60,  0.5, 0.5, 1);
    barrModel = new Model(gameWindow, "Objects/barr.obj", "Textures/barr.dds");
    fieldModel = new Model(gameWindow, "Objects/field.obj", "Textures/field.dds");
    fireBallModel = new Model(gameWindow, "Objects/fireball.obj", "Textures/fireball.dds");
    fireBallModel->setSimpleRingAnimation(0.2, 1, 4, 0, 0.3, 1);
    hpBarModel = new Model(gameWindow, "Objects/hp.obj", "Textures/hp.dds");
    //ini player warlock
    warlock = new Warlock(warlockModel, fireBallModel, hpBarModel, wsm::vec2(), 0, 1.1, 0.8, vec3(1, 1, 1), vec3(0, 0, 0),
                     2, 0.8, vec3(1, 1, 1), vec3(0, 0, 0));
    this->barrsCount = settings[2];
    this->botCount = 1;
    //ini enemy warlocks
    for (int i = 0; i < botCount; i++) {
        enemyWarlocks.push_back(new Warlock(warlockModel, fireBallModel, hpBarModel, wsm::vec2(), 0, 1.1, 0.8, vec3(1, 0.3, 0.3),
                                        vec3(0, 0, 0), 2, 1, vec3(1, 0.3, 0.3), vec3(0.8, 0.5, 0)));
        bots.push_back(Bot(gameWindow, enemyWarlocks[i], i));
    }
    magmaRadius = magmaModel->getVertexRadius();
    resetGame(0);
    gameWindow->addKeyListener(GLFW_KEY_ESCAPE);
    player = new Player(gameWindow, warlock);
}

void Game::resetGame(int addBots, int newBarrCount) {
    if (newBarrCount != -1) {
        barrs.resize(newBarrCount);
        barrsCount = newBarrCount;
    }
    if (addBots < 0) {
        delete(enemyWarlocks.back());
        enemyWarlocks.pop_back();
        bots.pop_back();
        botCount--;
    }
    if (addBots > 0) {
        enemyWarlocks.push_back(new Warlock(warlockModel, fireBallModel, hpBarModel, wsm::vec2(), 0, 1.1, 0.8, vec3(1, 0.3, 0.3),
                                            vec3(0, 0, 0), 2, 1, vec3(1, 0.3, 0.3), vec3(0.8, 0.5, 0)));
        bots.push_back(Bot(gameWindow, enemyWarlocks[botCount], botCount));
        botCount++;
    }
    gameWindow->setCameraLocationTo(0, 0);
    warlock->resetAll(wsm::vec2(3, 4), 0);
    srand(clock() * 2);
    for(int i = 0; i < botCount; i++) {
        float rotation = std::abs(((double)rand() / (double)RAND_MAX) * 6.28);
        float length = std::abs(((double)rand() / (double)RAND_MAX) * magmaRadius);
        float nx = std::cos(rotation) * length, ny = std::sin(rotation) * length;
        enemyWarlocks[i]->resetAll(wsm::vec2(nx, ny), 0);
        enemyWarlocks[i]->healthPoints /= botCount;
    }
    barrs.resize(barrsCount);
    srand(clock());
    for (size_t i = 0; i < barrsCount; i++) {
        float rotation = std::abs(((double)rand() / (double)RAND_MAX) * 6.28);
        float length = std::abs(((double)rand() / (double)RAND_MAX) * magmaRadius);
        float nx = std::cos(rotation) * length, ny = std::sin(rotation) * length;
        barrs[i].second = length;
        barrs[i].first = wsm::vec2(nx, ny);
    }
    magmaModel->resetVertexRadius();
    magmaRadius = magmaModel->getVertexRadius();
}

bool Game::gameProcess() {
    if (!onStart()) {
        return  false;
    }
    double time = clock();
    do {
        //save time of current frame
        double proctime = (clock() - time) / TIME_DIVISOR;
        time = clock();
        if (gameWindow->isKeyClicked(GLFW_KEY_ESCAPE)) {
            int res = onPause(true, std::string("Game Paused"));
            switch (res) {
                case 0 : {
                    return false;
                } break;
                case 1 : {
                    time = clock();
                } break;
                case 2: {
                    return true;
                }
            }
        }
        animationUpdate(proctime);
        gameLogic(proctime);
        gameDraw();
        if (gameOverCheck()) {
            std::string message;
            if (warlock->healthPoints > 0) {
                message = "YOU WIN";
                recordsUpdate();
            } else {
                message = "YOU LOSE";
            }
            int res = onPause(false, message);
            switch (res) {
                case 0 : {
                    return false;
                } break;
                case 1 : {
                    time = clock();
                } break;
                case 2 : {
                    return true;
                }
            }
        }
    } while (gameWindow->isWindowOpened());
    return false;
}


void Game::destroyGame() {
    gameWindow->closeWindow();
    delete(gameWindow);
    warlockModel->clear();
    delete(warlockModel);
    barrModel->clear();
    delete(barrModel);
    magmaModel->clear();
    delete(magmaModel);
    fieldModel->clear();
    delete(fieldModel);
    fireBallModel->clear();
    delete(fireBallModel);
    delete(warlock);
    hpBarModel->clear();
    delete(hpBarModel);
    delete(player);
    for (size_t i = 0; i < bots.size(); i++) {
        delete(enemyWarlocks[i]);
    }
}

void Game::gameLogic(double proctime) {
    player->playerStep(proctime * 5);
    relocate(warlock, proctime);
    for (int i = 0; i < botCount; i++) {
        if (enemyWarlocks[i]->healthPoints >= 0) {
            bots[i].botStep(warlock->position, magmaRadius);
            relocate(enemyWarlocks[i], proctime);
        }
    }
    barrClashCheck(warlock);
    magmaClashCheck(warlock, proctime);
    for (int i = 0; i < botCount; i++) {
        if (enemyWarlocks[i]->healthPoints >= 0) {
            clashCheck(warlock, enemyWarlocks[i], proctime);
            magmaClashCheck(enemyWarlocks[i], proctime);
            barrClashCheck(enemyWarlocks[i]);
        }
    }
    magmaRadius -= proctime * MAGMA_SPEED;
}

void Game::gameDraw() {
    gameWindow->drawStart();
    fieldModel->draw(0, 0, 0, 1, 1, vec3(1, 1, 1), vec3(0, 0, 0));
    magmaModel->draw(0, 0, 0, 3, 1, vec3(1, 1, 1), vec3(0, -0.2, -0.5));
    warlock->draw();
    for (int i = 0; i < botCount; i++) {
        enemyWarlocks[i]->draw();
    }
    for (int i = 0; i < barrsCount; i++) {
        if (barrs[i].second < magmaRadius) {
            barrModel->draw(barrs[i].first.x, barrs[i].first.y, 0, 2, 0.7);
        } else {
            barrModel->draw(barrs[i].first.x, barrs[i].first.y, 0, 2, std::max(0.75 - barrs[i].second + magmaRadius, 0.0),
                            vec3(1, 1, 1), vec3(std::min(0.0 + barrs[i].second - magmaRadius, 1.0), 0.1, 0.2));
        }
    }
    gameWindow->drawUIElements();
    gameWindow->drawEnd();
}

void Game::relocate(Warlock *warlock, double proctime) {
    if (warlock->isFire) {
        warlock->fireBallPosition = warlock->fireBallPosition + warlock->fireBallMoveVector * proctime * FIREBALL_SPEED;
    }
    //check clash with barrs
    for (int i = 0; i < barrs.size(); i++) {
        if (barrs[i].second < magmaRadius) {
            if (wsm::distance(warlock->position, barrs[i].first) < 1.2) {
                warlock->clearAdditionalVector();
                wsm::vec2 clashVector = wsm::get_move_vector(warlock->position, barrs[i].first);
                wsm::vec2 moveVector = wsm::get_vector_from_angle(warlock->getRotation());
                if (wsm::angle_bv(clashVector, moveVector) < PI / 2.1) {
                    wsm::vec2 vertical = wsm::get_near_vertical_vector(clashVector, moveVector);
                    float va = wsm::vector_angle(vertical);
                    if (wsm::nearest_arc(warlock->getRotation(), va)) {
                        warlock->addRotation(proctime * WARLOCK_ROTATION_SPEED);
                    } else {
                        warlock->addRotation(-proctime * WARLOCK_ROTATION_SPEED);
                    }
                    warlock->position = warlock->position + vertical * proctime * WARLOCK_SPEED;
                    return;
                }
            }
        }
    }
    if (warlock->position != warlock->targetPosition) {
        float alpha = wsm::vector_angle(wsm::get_move_vector(warlock->position, warlock->targetPosition));
        if (!(std::abs(alpha - warlock->getRotation()) <= 0.05) && !(std::abs(alpha - warlock->getRotation()) >= 2 * PI - 0.05)) {
            if (wsm::nearest_arc(warlock->getRotation(), alpha)) {
                warlock->addRotation(proctime * WARLOCK_ROTATION_SPEED);
            } else {
                warlock->addRotation(-proctime * WARLOCK_ROTATION_SPEED);
            }
        } else {
            warlock->setRotation(alpha);
        }
        warlock->position = warlock->position + wsm::get_vector_from_angle(warlock->getRotation()) * proctime * WARLOCK_SPEED;
        if (wsm::distance(warlock->position, warlock->targetPosition) < 0.1) {
            warlock->targetPosition = warlock->position;
        }
    }
    warlock->position = warlock->position + warlock->getAdditionalVector() * proctime * (WARLOCK_SPEED * 2);
}

void Game::clashCheck(Warlock *warlock1, Warlock *warlock2, double proctime) {
    fireClashCheck(warlock1, warlock2);
    fireClashCheck(warlock2, warlock1);
}

void Game::magmaClashCheck(Warlock *warlock, double proctime) {
    if (wsm::distance(warlock->position) > magmaRadius) {
        warlock->healthPoints -= MAGMA_DAMAGE * proctime;
    }
}

void Game::barrClashCheck(Warlock *warlock) {
    for (int i = 0; i < barrsCount; i++) {
        if (barrs[i].second < magmaRadius) {
            if (distance(warlock->fireBallPosition, barrs[i].first) < 0.8) {
                wsm::vec2 repulseVector;
                if (wsm::get_repulse_vector(repulseVector, wsm::get_move_vector(warlock->fireBallPosition,
                        barrs[i].first), warlock->fireBallMoveVector)) {
                    warlock->fireBallMoveVector = repulseVector;
                }
                return;
            }
        }
    }
}

void Game::fireClashCheck(Warlock *thisWarlock, Warlock *enemyWarlock) {
    if (thisWarlock->isFire) {
        if (wsm::distance(thisWarlock->fireBallPosition, enemyWarlock->position) < 0.9) {
            enemyWarlock->healthPoints -= FIREBALL_DAMAGE;
            thisWarlock->isFire = false;
            enemyWarlock->setAdditionalVector(wsm::normalize_vector(wsm::get_move_vector(thisWarlock->fireBallPosition,
                     enemyWarlock->position) + thisWarlock->fireBallMoveVector));
        }
    }
}

void Game::animationUpdate(double proctime) {
    warlockModel->animationUpdate();
    fireBallModel->animationUpdate();
    magmaModel->animationUpdate(proctime * MAGMA_SPEED);
}

bool Game::gameOverCheck() {
    if (warlock->healthPoints < 0) {
        return true;
    }
    for (int i = 0; i < botCount; i++) {
        if (enemyWarlocks[i]->healthPoints > 0) {
            return false;
        }
    }
    if (botCount > 0) {
        return true;
    } else {
        return false;
    }

}

int Game::onPause(bool isResumePossible, std::string message) {
    gameWindow->addLabel(0.6, 0.1, 60, message, vec3(0.2, 0.2, 0.9));
    int resume;
    if (isResumePossible) {
        resume = gameWindow->addButton(0.1, 0.9, 60, std::string("RESUME"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
    } else {
        if (botCount == 0) {
            warlock->healthPoints += 1;
        }
        if (warlock->healthPoints > 0) {
            resume = gameWindow->addButton(0.1, 0.9, 60, std::string("+1 bot"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
        } else {
            resume = gameWindow->addButton(0.1, 0.9, 60, std::string("-1 bot"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
        }
    }
    int exit = gameWindow->addButton(0.7, 0.9, 60, std::string("EXIT"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
    int restart = gameWindow->addButton(0.4, 0.9, 60, std::string("RESTART"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
    int settings = gameWindow->addButton(0.1, 0.1, 60, std::string("SETTINGS"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
    do {
        animationUpdate(0);
        gameDraw();
        if (gameWindow->isButtonClicked(exit)) {
            gameWindow->deleteUIElements();
            return 0;
        }
        if (gameWindow->isButtonClicked(restart)) {
            resetGame(0);
            if (!isResumePossible) {
                gameWindow->deleteUIElements();
                gameWindow->setCursorTo(0.5, 0.5);
                if (onStart()) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
        if (gameWindow->isButtonClicked(resume) || gameWindow->isKeyClicked(GLFW_KEY_ESCAPE)) {
            gameWindow->deleteUIElements();
            if (!isResumePossible) {
                gameWindow->setCursorTo(0.5, 0.5);
                if (warlock->healthPoints > 0) {
                    resetGame(1);
                } else {
                    resetGame(-1);
                }
                if (onStart()) {
                    return 1;
                } else {
                    return 0;
                }
            }
            return 1;
        }
        if (gameWindow->isButtonClicked(settings)) {
            gameWindow->deleteUIElements();
            if (onSettings()) {
                return 2;
            } else {
                return 1;
            }
        }
    } while (gameWindow->isWindowOpened());
}

bool Game::onSettings() {
    gameWindow->addLabel(0.4, 0.05, 60, std::string("SETTINGS"), vec3(0.4, 1, 0.3));
    gameWindow->addLabel(0.05, 0.2, 30, std::string("Difficulty"), vec3(0.2, 0.2, 0.9));
    gameWindow->addLabel(0.4, 0.2, 30, std::string("Screen Resolution"), vec3(0.2, 0.2, 0.9));
    gameWindow->addLabel(0.7, 0.2, 30, std::string("Monitor Mode"), vec3(0.2, 0.2, 0.9));
    int resume = gameWindow->addButton(0.4, 0.9, 60, std::string("RESUME"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));

    const std::string difficulty[4] = {" 0 barrs  ", " 20 barrs ", " 30 barrs ", " 80 barrs "};
    const int difficultyInt[4] = {0, 20, 30, 80};
    int difficultyButtonID[4];
    for (int i = 0; i < 4; i++) {
        difficultyButtonID[i] = gameWindow->addButton(0.05, 0.3 + i * 0.1, 30, difficulty[i], vec3(1, 1, 1), vec4(0.6, 0.2, 0.8, 0.7));
        if (barrsCount == difficultyInt[i]) {
            gameWindow->changeButtonActive(difficultyButtonID[i]);
        }
    }

    const std::string resolutions[4] = {"   1024 * 576   ", "   1280 * 720   ", "   1366 * 768   ", "   1600 * 900   "};
    const int resolutionsInt[4][2] = {{1024, 576}, {1280, 720}, {1366, 768}, {1600, 900}};
    int resolutionsButtonID[4];
    const std::string targetResolution =
            std::to_string(gameWindow->getWidth()).append(std::string(" * ")).append(std::to_string(gameWindow->getHeight()));
    for (int i = 0; i < 4; i++) {
        resolutionsButtonID[i] = gameWindow->addButton(0.4, 0.3 + i * 0.1, 30, resolutions[i], vec3(1, 1, 1), vec4(0.6, 0.2, 0.8, 0.7));
        if (resolutions[i].find(targetResolution) != std::string::npos) {
            gameWindow->changeButtonActive(resolutionsButtonID[i]);
        }
    }

    const std::string monitorMode[2] = {"  windowed  ", " fullscreen "};
    int monitorModeButtonID[2];
    monitorModeButtonID[0] = gameWindow->addButton(0.7, 0.3, 30, monitorMode[0], vec3(1, 1, 1), vec4(0.6, 0.2, 0.8, 0.7));
    monitorModeButtonID[1] = gameWindow->addButton(0.7, 0.4, 30, monitorMode[1], vec3(1, 1, 1), vec4(0.6, 0.2, 0.8, 0.7));
    gameWindow->changeButtonActive(monitorModeButtonID[fullScreen]);

    do {
        animationUpdate(0);
        gameDraw();
        for (int i = 0; i < 4; i++) {
            if (gameWindow->isButtonClicked(difficultyButtonID[i])) {
                gameWindow->deleteUIElements();
                resetGame(0, difficultyInt[i]);
                Settings settings("warlocks.cfg");
                settings.set("barrs", std::to_string(difficultyInt[i]));
                return false;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (gameWindow->isButtonClicked(resolutionsButtonID[i])) {
                gameWindow->deleteUIElements();
                Settings settings("warlocks.cfg");
                settings.set("width", std::to_string(resolutionsInt[i][0]));
                settings.set("height", std::to_string(resolutionsInt[i][1]));
                return true;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (gameWindow->isButtonClicked(monitorModeButtonID[i])) {
                Settings settings("warlocks.cfg");
                settings.set("fullscreen", std::to_string(i));
                return true;
            }
        }
        if (gameWindow->isButtonClicked(resume) || gameWindow->isKeyClicked(GLFW_KEY_ESCAPE)) {
            gameWindow->deleteUIElements();
            return false;
        }
    } while (gameWindow->isWindowOpened());
}

std::vector<int> Game::getSettings() {
    std::vector<int> temp(4);
    Settings settings("warlocks.cfg");
    int width = settings.get("width") != "" ? std::stoi(settings.get("width")) : 1280;
    int height = settings.get("height") != "" ? std::stoi(settings.get("height")) : 720;
    int barrs = settings.get("barrs") != "" ? std::stoi(settings.get("barrs")) : 30;
    bool fullScreen = settings.get("fullscreen") != "" ? std::stoi(settings.get("fullscreen")) : 0;
    float aspect = (float)width / (float)height;
    if (aspect < 1.8 && aspect > 1.7) {
        temp[0] = width;
        temp[1] = height;
    }
    if (barrs > 100 || barrs < 0) {
        temp[2] = 30;
    } else {
        temp[2] = barrs;
    }
    temp[3] = fullScreen;
    return temp;
}

bool Game::onStart() {
    gameWindow->addLabel(0.3, 0.1, 90, std::string("WARLOCKS"), vec3(1, 0.5, 0.1));
    gameWindow->addLabel(0.2, 0.3, 40, getRecords(), vec3(1, 0.2, 0.5));
    int start = gameWindow->addButton(0.4, 0.9, 60, std::string("START"), vec3(1, 1, 1), vec4(0, 1, 0.5, 0.7));
    do {
        animationUpdate(0);
        gameDraw();
        if (gameWindow->isButtonClicked(start)) {
            gameWindow->deleteUIElements();
            return 1;
        }
    } while (gameWindow->isWindowOpened() || gameWindow->isKeyClicked(GLFW_KEY_ESCAPE));
    return 0;
}

void Game::recordsUpdate() {
    Settings settings("warlocks.rcd");
    int lvl = settings.get("lvl") != "" ? std::stoi(settings.get("lvl")) : 0;
    if (botCount > lvl) {
        settings.set("lvl", std::to_string(botCount));
    }
}

std::string Game::getRecords() {
    Settings settings("warlocks.rcd");
    int lvl = settings.get("lvl") != "" ? std::stoi(settings.get("lvl")) : 0;
    return std::string("Record: defeated ").append(std::to_string(lvl)).append(std::string(" enemy!"));
}
Game::~Game() {
    destroyGame();
}
