#pragma once
#include "GameWindow.h"
#include "Model.h"
#include <vector>
#include <ctime>
#include <glm.hpp>
#include "WMath.h"


class Warlock {
public:
    /**
     * position - Position of warlock
     * targetPosition - Position where warlock wnts to go
     * fireBallMoveVector - Normal vector to relocate fireball
     * fireBallPosition - Position of fireball
     */
    wsm::vec2 position, targetPosition, fireBallMoveVector, fireBallPosition;
    /**
     * healthPoints - Health points of warlock (100 - max)
     */
    float healthPoints;
    /**
     * isFire - true if fireball exist
     */
    bool isFire;
    /**
     * Construct warlock
     * \param warlockModel Model of warlock to draw
     * \param fireBallModel Model of fireball to draw
     * \param hpBarModel Model of hp bar to draw
     * \param postition Postition to spawn
     * \param rotation Rotation to spawn (in radians)
     * \param warlockBright Warlock model bright
     * \param warlockTransparency Warlock model transparency (from 0 to 1)
     * \param warlockFilter Color filter of warlock model
     * \param warlockColor Additional color of warlock model
     * \param fireBallBright Fireball model bright
     * \param fireBallTransparency Fireball model transparency (from 0 to 1)
     * \param fireBallFilter Color filter of fireball model
     * \param fireBallColor Additional color of fireball model
     */
    Warlock(Model *warlockModel, Model *fireBallModel, Model *hpBarModel, wsm::vec2 position, float rotation, float warlockBright = 1,
            float warlockTransparency = 1, vec3 warlockFilter = vec3(1, 1, 1), vec3 warlockColor = vec3(0, 0, 0),
            float fireBallBright = 1, float fireBallTransparency = 1, vec3 fireBallFilter = vec3(1, 1, 1),
            vec3 colorFireBall = vec3(0, 0, 0));
    /**
     * Draw Warlock, warlock hp bar and his fireball if it exist
     */
    void draw();
    /**
     * \return rotation of warlock (in radians)
     */
    float getRotation();
    /**
     * \param angle Angle to set rotation (in radians)
     */
    void setRotation(float angle);
    /**
     * \param angle Angle to add to rotation (in radians)
     */
    void addRotation(float angle);
    /**
     * Create fireball near warlock and turn it into vector to
     * \param to vector to turn
     */
    void setFire(wsm::vec2 to);
    /**
     * Sets additional vector to standart warlock move,
     * It lives only one second then deletes
     * \param vector Vector to add
     */
    void setAdditionalVector(wsm::vec2 vector);
    /**
     * \return Additional vector, if it dont exist return (0, 0)
     */
    wsm::vec2 getAdditionalVector();
    /**
     * Deletes additional vector
     */
    void clearAdditionalVector();
    /**
     * Reset warlock hp and positions
     * \param postion Position to spawn
     * \param rotation Rotation of warlock
     */
    void resetAll(wsm::vec2 position, float rotation);

private:
    Model *warlockModel, *fireBallModel, *hpBarModel;
    float timer, rotation;
    wsm::vec2 additionVector;
    float warlockBright, warlockTransparency, fireBallBright, fireBallTransparency;
    vec3 warlockFilter, warlockColor, fireBallFilter, fireBallColor;
};
