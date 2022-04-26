/**
 * EDA-Man
 *
 * @file Player.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "Robot.h"

#define VELOCIDAD_PLAYER 0.32f

class Player : public Robot
{
public:
	Player(std::string id, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);

private:
	GameView* gameView;
};



#endif // !PLAYER_H