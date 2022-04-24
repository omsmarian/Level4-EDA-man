#include "Orange.h"

Orange::Orange(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->coordinates = spawn;
}


void Orange::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->movement({ 0,0 });
	this->setDisplay(23);
}

void Orange::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	printf("time = %f\n", this->timeUpdate);
	if (this->timeUpdate < 7)
		this->persecucion({ -1.15, -1.7 }, deltaTime);
	if (7 <= this->timeUpdate && this->timeUpdate < 27)
		this->persecucion(this->getPacManPosition(), deltaTime);
	if (this->timeUpdate >= 20)
	{
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
