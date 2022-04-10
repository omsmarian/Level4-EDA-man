#include "Player.h"

void Player::start(MQTTClient* client, GameModel* model)
{
    this->robotId = "robot1";
    this->mqttClient = client;
    this->gameModel = model;
    this->setDisplay(1);
    this->liftTo(0,0);
    RobotSetpoint originPlace = {0,0,0};
    this->coordenates = this->getMazePosition (originPlace);
}

void Player::movement(int posX, int posZ)
{
    this->coordenates = {this->coordenates.x + posX, this->coordenates.y + posZ};
    RobotSetpoint lugar = this->getRobotSetpoint(this->coordenates, 0);
    this->setSetpoint(lugar);
}

void Player::reset()
{
    this->liftTo(0, 0);
}
