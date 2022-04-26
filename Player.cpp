/**
 * EDA-Man
 *
 * @file Player.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include "Player.h"
/**
 * @brief player
 *
 * @param  id and spawn coordinates
 */
Player::Player(std::string id, Vector2 spawn)
{
	this->robotId = id;
	this->inicialPosition = spawn;
	this->coordinates = spawn;
}
/**
 * @brief inizialites player variables
 *
 * @param client model
 */
void Player::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->originalImage = 1;
	this->setDisplay(this->originalImage);
	this->liftTo(this->converter(this->coordinates));
}
/**
 * @brief Updates player position
 *
 * @param deltaTime
 */
void Player::update(float deltaTime)
{
	Vector2 increment = { 0,0 };
	if (this->direction == DirectionUp)
	{
		increment.y = deltaTime * VELOCIDAD_PLAYER;
		this->setpoint.rotation = 0;
	}
	else if (this->direction == DirectionDown)
	{
		increment.y = -deltaTime * VELOCIDAD_PLAYER;
		this->setpoint.rotation = 180;
	}
	else if (this->direction == DirectionRight)
	{
		increment.x = deltaTime * VELOCIDAD_PLAYER;
		this->setpoint.rotation = 90;
	}
	else if (this->direction == DirectionLeft)
	{
		increment.x = -1 * deltaTime * VELOCIDAD_PLAYER;
		this->setpoint.rotation = 270;
	}
	else if (this->direction == DirectionNone)
	{
		increment.x = 0;
		increment.y = 0;
	}
	this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

	if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
	{
		this->movement(increment);
	}
}