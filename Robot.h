/**
 * Robot base class.
 *
 * @copyright Copyright (C) 2022
 * @author Marc S. Ressl
 */

#ifndef _ROBOT_H
#define _ROBOT_H

class GameModel;

#include <string>

#include <raylib.h>

#include "MQTTClient.h"
#include "GameModel.h"

/**
 * @brief Robot controller setpoint.
 * 
 * position.x is world position x
 * position.y is world position z
 * rotation is world rotation y
 */
struct Setpoint
{
    Vector2 position;
    float rotation;
};

/**
 * @brief Indicates a robot direction.
 * 
 */
enum Direction
{
    DirectionNone,
    DirectionUp,
    DirectionRight,
    DirectionDown,
    DirectionLeft
};

class Robot
{
public:
    Robot();
    virtual ~Robot();

    virtual void start(MQTTClient* client, GameModel* model);
    void update(float deltaTime);
    void movement(Vector2 position);

protected:
    // These variables should be set by you...
    MQTTClient *mqttClient;
    GameModel *gameModel;
    std::string robotId;

    Vector2 posXY;
    float speed;
    Direction robotDirection;
    int imageIndex;
    Image displayImages;

    bool isMoving;
    Setpoint setpoint;

    // Add your variables here...

    Vector2 getTilePosition(Setpoint setpoint);
    Setpoint getSetpoint(Vector2 tilePosition);
    void setSetpoint(Setpoint setpoint);
    void liftTo(Vector3 destination);
    void setDisplay();
    void setEyes(Color leftEye, Color rightEye);
};

#endif
