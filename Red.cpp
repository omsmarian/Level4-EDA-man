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
	this->liftTo(this->converter(this->coordinates));
	this->setDisplay(289);
}
