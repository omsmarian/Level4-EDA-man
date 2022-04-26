/**
 * EDA-Man
 *
 * @file Cyan.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */


#ifndef CYAN_H
#define CYAN_H

#include "Ghost.h"

class Cyan : public Ghost
{
public:
	Cyan(std::string robotId, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);
	Vector2 getDestinationCyan();

private:

	int lastDirection;
};

#endif // !CYAN_H