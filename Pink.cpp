/**
 * EDA-Man
 *
 * @file Pink.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */


#include "Pink.h"
 /**
  * @brief sets initial values for ghost
  *
  * @param robot id and spawn coordinates
  */
Pink::Pink(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->inicialPosition = spawn;
	this->coordinates = spawn;
	this->lastDirection = 0;
}
/**
 * @brief  inicializes ghost values
 *
 * @param mosquito client and game model
 */
void Pink::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->originalImage = 19;
	this->setDisplay(this->originalImage);
	this->liftTo(converter(this->inicialPosition));
}
/**
 * @brief sets ghost persecution and chase time
 *
 * @param deltaTime
 */
void Pink::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])	//scatter
		this->persecucion({ -1.15,1.5 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate <
		(TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))	//chase
		this->persecucion(this->getDestinationPink(), deltaTime);
	if (this->timeUpdate >= TIME_ARRAY[this->timeIndex + 1])	//update chase scatter seconds
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}
/**
 * @brief sets the destination of the Pink ghost
 *
 * @return vector of destiny
 */

Vector2 Pink::getDestinationPink()
{
	Vector2 destino = this->gameModel->getPosition(0);
	int direction = this->gameModel->getPlayerDirection(0);

	if (direction == DirectionUp)
	{
		destino.y += 0.4;
		this->lastDirection = direction;
	}
	else if (direction == DirectionDown)
	{
		destino.y += -0.4;
		this->lastDirection = direction;
	}
	else if (direction == DirectionRight)
	{
		destino.x += 0.4;
		this->lastDirection = direction;
	}
	else if (direction == DirectionLeft)
	{
		destino.x += -0.4;
		this->lastDirection = direction;
	}
	else if (direction == DirectionNone)
	{
		if (this->lastDirection == DirectionUp)
		{
			destino.y += 0.4;
		}
		else if (this->lastDirection == DirectionDown)
		{
			destino.y += -0.4;
		}
		else if (this->lastDirection == DirectionRight)
		{
			destino.x += 0.4;
		}
		else if (this->lastDirection == DirectionLeft)
		{
			destino.x += -0.4;
		}
	}

	return destino;
}