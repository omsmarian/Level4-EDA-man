/**
 * EDA-Man
 *
 * @file Orange.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include "Orange.h"
 /**
  * @brief sets initial values for ghost
  *
  * @param robot id and spawn coordinates
  */
Orange::Orange(std::string robotId, Vector2 spawn)
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

void Orange::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->originalImage = 23;
	this->setDisplay(this->originalImage);
	this->liftTo(converter(this->inicialPosition));
}
/**
 * @brief sets ghost persecution and chase time
 *
 * @param deltaTime
 */
void Orange::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])	//scatter
		this->persecucion({ -1.15, -1.7 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate <
		(TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))
		this->persecucion(this->getPacManPosition(), deltaTime);	//chase
	if (this->timeUpdate >= TIME_ARRAY[this->timeIndex + 1])//update chase scatter seconds
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}
/**
 * @brief sets the destination of the Orange ghost
 *
 * @return vector of destiny
 */

Vector2 Orange::getPacManPosition()
{
	Vector2 pacMan = this->gameModel->getPosition(0);
	Vector2 destino = pacMan;
	float norma = Vector2Distance(pacMan, this->coordinates);
	if (norma <= 0.8)
		destino = { -1.15, -1.7 };

	return destino;
}
