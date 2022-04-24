#include "Red.h"

Red::Red(std::string robotId, Vector2 spawn)
{
	this->robotId = robotId;
	this->coordinates = spawn;
}

void Red::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->movement({ 0,0 });
	this->setDisplay(17);
}

void Red::update(float deltaTime)
{
	this->timeUpdate += deltaTime;
	printf("time = %f\n", this->timeUpdate);
	if (this->timeUpdate < 7)
		this->persecucion({ 1.15,1.5 }, deltaTime);
	if (7 <= this->timeUpdate && this->timeUpdate < 27)
		this->persecucion(this->getPacManPosition(), deltaTime);
	if (this->timeUpdate >= 20)
	{
		this->timeUpdate = 0;
	}
}


Vector2 Red::getPacManPosition()
{
	return this->gameModel->getPosition(0);
}
