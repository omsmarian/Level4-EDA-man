#include "Orange.h"

Orange::Orange(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->inicialPosition = { spawn.x, 0, spawn.y };
	this->coordinates = spawn;
}


void Orange::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->movement({ 0,0 });
	this->setDisplay(23);
	this->liftTo(this->inicialPosition);
}

void Orange::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	printf("time = %f\n", this->timeUpdate);
	if (this->timeUpdate < TIME_ARRAY[this->timeIndex])
		this->persecucion({ -1.15, -1.7 }, deltaTime);
	if (TIME_ARRAY[this->timeIndex] <= this->timeUpdate && this->timeUpdate < (TIME_ARRAY[this->timeIndex] + TIME_ARRAY[this->timeIndex + 1]))
		this->persecucion(this->getPacManPosition(), deltaTime);
	if (this->timeUpdate >= TIME_ARRAY[this->timeIndex + 1])
	{
		this->timeIndex += 2;
		this->timeUpdate = 0;
	}
}

Vector2 Orange::getPacManPosition()
{
	Vector2 pacMan = this->gameModel->getPosition(0);
	Vector2 destino = pacMan;
	float norma = Vector2Distance(pacMan, this->coordinates);
	if (norma <= 0.8)
		destino = { -1.15, -1.7 };

	return destino;
}
