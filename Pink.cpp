#include "Pink.h"

Pink::Pink(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->inicialPosition = spawn;
	this->coordinates = spawn;
	this->lastDirection = 0;
}

void Pink::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->movement({ 0,0 });
	this->setDisplay(19);
}

void Pink::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])
		this->persecucion({ -1.15,1.5 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate < (TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))
		this->persecucion(this->getDestinationPink(), deltaTime);
	if (this->timeUpdate >= TIME_ARRAY[this->timeIndex + 1])
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}

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