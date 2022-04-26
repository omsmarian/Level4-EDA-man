/**
 * EDA-Man
 *
 * @file Red.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include "Red.h"
 /**
  * @brief sets initial values for ghost
  *
  * @param robot id and spawn coordinates
  */
Red::Red(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->inicialPosition = spawn;
	this->coordinates = spawn;
}
/**
 * @brief  inicializes ghost values
 *
 * @param mosquito client and game model
 */
void Red::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->originalImage = 17;
	this->setDisplay(this->originalImage);
	this->liftTo(converter(this->inicialPosition));
}
/**
 * @brief sets ghost persecution and chase time
 *
 * @param deltaTime
 */
void Red::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])//scatter
		this->persecucion({ 1.15,1.5 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate <
		(TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1])) //chase
		this->persecucion(this->getPacManPosition(), deltaTime);
	if (this->timeUpdate >= TIME_ARRAY[this->timeIndex + 1])	//update chase scatter seconds
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}

/**
 * @brief sets the destination of the Red ghost
 *
 * @return vector of destiny
 */

Vector2 Red::getPacManPosition()
{
	return this->gameModel->getPosition(0);
}
