#include "Cyan.h"

Cyan::Cyan(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->coordinates = spawn;
	this->lastDirection = 0;
}


void Cyan::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->movement({ 0,0 });
	this->setDisplay(21);
}

void Cyan::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	printf("time = %f\n", this->timeUpdate);
	if (this->timeUpdate < 7)
		this->persecucion({ 1.25,-1.7 }, deltaTime);
	if (7 <= this->timeUpdate && this->timeUpdate < 27)
		this->persecucion(this->getDestinationCyan(), deltaTime);
	if (this->timeUpdate >= 20)
	{
		this->timeUpdate = 0;
	}
}


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