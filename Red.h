/**
 * EDA-Man
 *
 * @file Red.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef RED_H
#define RED_H

#include "Ghost.h"

class Red : public Ghost
{
public:
	Red(std::string robotId, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);
	Vector2 getPacManPosition();
};

#endif // !RED_H
