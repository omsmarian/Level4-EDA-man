/**
 * EDA-Man
 *
 * @file Orange.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef ORANGE_H
#define ORANGE_H

#include "Ghost.h"

class Orange : public Ghost
{
public:
	Orange(std::string robotId, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);
	Vector2 getPacManPosition();
};

#endif // !ORANGE_H