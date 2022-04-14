#include "Player.h"

Player::Player(std::string id, Vector2 spawn)
{
	this->robotId = id;
	this->coordinates = this->getTilePosition(this->getSetpoint(spawn));
	
}

void Player::start(MQTTClient* client, GameModel* model)
{
	this->mqttClient = client;
	this->gameModel = model;
	this->liftTo(this->converter(this->coordinates));
	this->setDisplay(1);
}

