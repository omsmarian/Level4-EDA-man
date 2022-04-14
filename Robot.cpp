/**
 * Robot base class.
 *
 * @copyright Copyright (C) 2022
 * @author Marc S. Ressl
 */

#include <cstring>  // std::memcpy
#include <iostream>
#include <vector>

#include "Robot.h"

using namespace std;

/**
 * @brief Construct a new Robot:: Robot object
 * 
 */
Robot::Robot()
{
    // To-Do: set your path!
    displayImages = LoadImage("../../../RobotImages.png");
}

/**
 * @brief Destroy the Robot:: Robot object
 * 
 */
Robot::~Robot()
{
    UnloadImage(displayImages);
}

/**
 * @brief Initializes the robot for a game.
 *
 */
void Robot::start(MQTTClient* client, GameModel* model)
{
    
}

/**
 * @brief Updates the robot for the current frame.
 *
 * @param deltaTime The number of seconds since the last frame.
 */
void Robot::update(float deltaTime)
{
    Vector2 increment = { 0,0 };
    if (this->direction == DirectionUp)
    {
        increment.y = deltaTime * VELOCIDAD;
        this->setpoint.rotation = 0;
    }
    else if (this->direction == DirectionDown)
    {
        increment.y = -deltaTime * VELOCIDAD;
        this->setpoint.rotation = 180;
    }
    else if (this->direction == DirectionRight)
    {
        increment.x = deltaTime * VELOCIDAD;
        this->setpoint.rotation = 90;
    }
    else if (this->direction == DirectionLeft)
    {
        increment.x = -1 * deltaTime * VELOCIDAD;
        this->setpoint.rotation = 270;
    }
    else if (this->direction == DirectionNone)
    {
        increment.x = 0;
        increment.y = 0;
    }
    this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y};
    if(this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
        this->movement(increment);
}

/**
 * @brief Converts a setpoint to a tile position in maze coordinates.
 *
 * @param setpoint The setpoint
 * @return Vector2 The tile position
 */
Vector2 Robot::getTilePosition(Setpoint setpoint)
{
    Vector2 mazePosition;

    mazePosition.x = (10.0F * (1.4F + setpoint.position.x));
    mazePosition.y = (10.0F * (1.8F - setpoint.position.y));

    return mazePosition;
}

/**
 * @brief Converts a tile position in maze coordinates to a setpoint.
 *
 * @param tilePosition The tile position
 * @return Setpoint The setpoint (using current robot rotation)
 */
Setpoint Robot::getSetpoint(Vector2 tilePosition)
{
    Setpoint setpoint;
    setpoint.position.x = -1.4F + 0.1F * tilePosition.x;
    setpoint.position.y = 1.8F - 0.1F * tilePosition.y;
    setpoint.rotation = this->setpoint.rotation;

    return setpoint;
}

/**
 * @brief Sets the robot controller setpoint.
 *
 * @param setpoint The setpoint
 */
void Robot::setSetpoint(Setpoint setpoint)
{
    this->setpoint = setpoint;

    vector<char> payload(12);

    *((float *)&payload[0]) = setpoint.position.x;
    *((float *)&payload[4]) = setpoint.position.y;
    *((float *)&payload[8]) = setpoint.rotation;

    mqttClient->publish(robotId + "/pid/setpoint/set", payload);
}

/**
 * @brief Lifts the robot to a destination coordinate
 *
 * @param destination The destination coordinate (x: left-right, y: up-down, z: forward-back)
 */
void Robot::liftTo(Vector3 destination)
{
    vector<char> payload(12);

    *((float *)&payload[0]) = destination.x;
    *((float *)&payload[4]) = destination.y;
    *((float *)&payload[8]) = destination.z;

    mqttClient->publish("hook/" + robotId + "/cmd", payload);
}

/**
 * @brief Sets image on the display
 *
 * @param imageIndex The index of the image (see RobotImages.png)
 */
void Robot::setDisplay(int imageIndex)
{
    Rectangle selectRectangle = {16.0F * imageIndex, 0, 16, 16};
    Image selectedImage = ImageFromImage(displayImages, selectRectangle);

    const int dataSize = 16 * 16 * 3;
    vector<char> payload(dataSize);
    memcpy(payload.data(), selectedImage.data, dataSize);

    UnloadImage(selectedImage);

    mqttClient->publish(robotId + "/display/lcd/set", payload);
}

/**
 * @brief Set robot eyes.
 *
 * @param leftEye Left eye color
 * @param rightEye Right eye color.
 */
void Robot::setEyes(Color leftEye, Color rightEye)
{
    vector<char> payload(3);
    payload[0] = leftEye.r;
    payload[1] = leftEye.g;
    payload[2] = leftEye.b;
    mqttClient->publish(robotId + "/display/leftEye/set", payload);

    payload[0] = rightEye.r;
    payload[1] = rightEye.g;
    payload[2] = rightEye.b;
    mqttClient->publish(robotId + "/display/rightEye/set", payload);
}

Vector3 Robot::converter(Vector2 vector)
{
    Vector3 variable = { 0,0,0 };
    variable.x = vector.x;
    variable.y = 0;
    variable.z = vector.y;
    return variable;
}


void Robot::movement(Vector2 addCoordinates)
{
    this->coordinates.x += addCoordinates.x;
    this->coordinates.y += addCoordinates.y;
    this->setpoint.position = this->coordinates;
    this->setSetpoint(this->setpoint);
}

void Robot::setDirection(Direction currentDirection)
{
    this->direction = currentDirection;
}