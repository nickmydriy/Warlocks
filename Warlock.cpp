#include "Warlock.h"

Warlock::Warlock(Model *warlockModel, Model *fireBallModel, Model *hpBarModel, wsm::vec2 position, float rotation, float warlockBright,
                 float warlockTransparency, vec3 warlockFilter, vec3 warlockColor, float fireBallBright,
                 float fireBallTransparency, vec3 fireBallFilter, vec3 fireBallColor) {
    this->warlockModel = warlockModel;
    this->fireBallModel = fireBallModel;
    this->hpBarModel = hpBarModel;

    this->warlockTransparency = warlockTransparency;
    this->warlockBright = warlockBright;
    this->warlockColor = warlockColor;
    this->warlockFilter = warlockFilter;

    this->fireBallTransparency = fireBallTransparency;
    this->fireBallBright = fireBallBright;
    this->fireBallColor = fireBallColor;
    this->fireBallFilter = fireBallFilter;

    resetAll(position, rotation);
}

void Warlock::draw() {
    if (healthPoints >= 0) {
        warlockModel->draw(position.x, position.y, rotation, warlockBright,
                           warlockTransparency, warlockFilter, warlockColor);

        if (isFire) {
            fireBallModel->draw(fireBallPosition.x, fireBallPosition.y, 0,
                                fireBallBright, fireBallTransparency, fireBallFilter, fireBallColor);
        }

        hpBarModel->setCompressX(0.55 * (healthPoints / 100));
        hpBarModel->draw(position.x, position.y, 0, 2, 0.7);
    }
}

void Warlock::setFire(wsm::vec2 to) {
    isFire = true;
    fireBallMoveVector = wsm::get_move_vector(position, to);
    fireBallPosition = position;
}

void Warlock::setAdditionalVector(wsm::vec2 vector) {
    if (clock() < timer) {
        additionVector = vector + additionVector * ((timer - clock()) / 2000);
        timer = clock() + 1000;
    } else {
        additionVector = vector;
        timer = clock() + 1000;
    }
}

wsm::vec2 Warlock::getAdditionalVector() {
    if (clock() < timer) {
        return additionVector;
    } else {
        return wsm::vec2(0, 0);
    }
}

void Warlock::clearAdditionalVector() {
    timer = clock();
}

float Warlock::getRotation() {
    return rotation;
}

void Warlock::setRotation(float angle) {
    rotation = angle;
    if (rotation > 2 * PI) {
        rotation -= 2 * PI;
    }
    if (rotation < 0) {
        rotation += 2 * PI;
    }
}

void Warlock::addRotation(float angle) {
    rotation += angle;
    if (rotation > 2 * PI) {
        rotation -= 2 * PI;
    }
    if (rotation < 0) {
        rotation += 2 * PI;
    }
}
void Warlock::resetAll(wsm::vec2 position, float rotation) {
    this->position = position;
    this->targetPosition = position;
    this->rotation = rotation;
    additionVector = wsm::vec2(0, 0);
    healthPoints = 100;
    isFire = false;
    timer = clock();
}