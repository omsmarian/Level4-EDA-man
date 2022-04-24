#include "Pink.h"

Pink::Pink(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
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
	printf("time = %f\n", this->timeUpdate);
	if (this->timeUpdate < 7)
		this->persecucion({ -1.15,1.5 }, deltaTime);
	if (7 <= this->timeUpdate && this->timeUpdate < 27)
		this->persecucion(this->getDestinationPink(), deltaTime);
	if (this->timeUpdate >= 20)
	{
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