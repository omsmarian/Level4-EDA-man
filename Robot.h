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

#define VELOCIDAD 0.64f
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
    void setDirection(Direction currentDirection);

protected:
    // These variables should be set by you...
    MQTTClient *mqttClient;
    GameModel *gameModel;
    std::string robotId;

    Image displayImages;

    Vector2 coordinates;

    Direction direction;
    bool isMoving;
    Setpoint setpoint;

    // Add your variables here...
    Vector2 getTilePosition(Setpoint setpoint);
    Setpoint getSetpoint(Vector2 tilePosition);
    void setSetpoint(Setpoint setpoint);
    void liftTo(Vector3 destination);
    void setDisplay(int imageIndex);
    void setEyes(Color leftEye, Color rightEye);
    Vector3 converter(Vector2 vector);
    void movement(Vector2 coordinates);
};

#endif
