/**
 * EDA-Man
 *
 * @file Robot.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
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
	DirectionUp,
	DirectionLeft,
	DirectionRight,
	DirectionDown,
	DirectionNone
};

class Robot
{
public:
	Robot();
	virtual ~Robot();

	virtual void start(MQTTClient* client, GameModel* model) = 0;
	virtual void update(float deltaTime) = 0;

	void setDirection(Direction currentDirection);
	Vector2 getTilePosition(Setpoint setpoint);
	Vector2 getCoordinates();
	Direction getDirection();
	void setCoordinates(Vector2 nextCoords);
	void setSetpoint(Setpoint setpoint);
	void movement(Vector2 coordinates);
	void resetRobot();
	void setImageIndex(int image);
	int getOriginalImage();
	void liftTo(Vector3 destination);

protected:
	MQTTClient* mqttClient;
	GameModel* gameModel;
	std::string robotId;

	Image displayImages;
	int originalImage;

	Vector2 coordinates;

	Vector2 inicialPosition;

	Direction direction;
	bool isMoving;
	Setpoint setpoint;

	Setpoint getSetpoint(Vector2 tilePosition);
	void setDisplay(int imageIndex);
	void setEyes(Color leftEye, Color rightEye);
	Vector3 converter(Vector2 vector);

};

#endif
