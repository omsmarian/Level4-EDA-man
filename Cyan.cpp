/**
 * EDA-Man
 *
 * @file Cyan.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */


#include "Cyan.h"
 /**
  * @brief sets initial values for ghost
  *
  * @param robot id and spawn coordinates
  */
Cyan::Cyan(std::string robotId, Vector2 spawn)
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

void Cyan::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->originalImage = 21;
	this->setDisplay(this->originalImage);
	this->liftTo(converter(this->inicialPosition));
}
/**
 * @brief sets ghost persecution and chase time
 *
 * @param deltaTime
 */
void Cyan::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])				//chase
		this->persecucion({ 1.25,-1.7 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate <
		(TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))		//scatter
		this->persecucion(this->getDestinationCyan(), deltaTime);
	if (this->timeUpdate >= (TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))		//update chase scatter seconds
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}
/**
 * @brief sets the destination of the Cyan ghost
 *
 * @return vector of destiny
 */

Vector2 Cyan::getDestinationCyan()
{
	Vector2 eje = this->gameModel->getPosition(0);
	int direction = this->gameModel->getPlayerDirection(0);

	if (direction == DirectionUp)
	{
		eje.y += 0.2;
		this->lastDirection = direction;
	}
	else if (direction == DirectionDown)
	{
		eje.y += -0.2;
		this->lastDirection = direction;
	}
	else if (direction == DirectionRight)
	{
		eje.x += 0.2;
		this->lastDirection = direction;
	}
	else if (direction == DirectionLeft)
	{
		eje.x += -0.2;
		this->lastDirection = direction;
	}
	else if (direction == DirectionNone)
	{
		if (this->lastDirection == DirectionUp)
		{
			eje.y += 0.2;
		}
		else if (this->lastDirection == DirectionDown)
		{
			eje.y += -0.2;
		}
		else if (this->lastDirection == DirectionRight)
		{
			eje.x += 0.2;
		}
		else if (this->lastDirection == DirectionLeft)
		{
			eje.x += -0.2;
		}
	}

	Vector2 redPosition = this->gameModel->getPosition(1);

	float parteY = redPosition.y - eje.y;
	float parteX = redPosition.x - eje.x;

	return { eje.x - parteX, eje.y - parteY };
}