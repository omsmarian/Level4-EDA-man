#include "Player.h"

Player::Player(std::string id, Vector2 spawn)
{
	this->robotId = id;
	this->inicialPosition = spawn;
	this->coordinates = spawn;
}

void Player::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->setDisplay(1);
	this->liftTo(this->converter(this->coordinates));
}

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