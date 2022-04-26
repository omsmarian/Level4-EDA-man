/**
 * EDA-Man
 *
 * @file Ghost.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef GHOST_H
#define GHOST_H

#include "Robot.h"

#define VELOCIDAD_GHOST 0.3f

const float TIME_ARRAY[8] = { 7.0,20.0,7.0,20.0,5.0,20.0,5.0,999999999.0 };

enum ghostMovement
{
	Chase,
	Scatter,
	Fightened
};


class Ghost : public Robot
{
public:

private:
	void verification(Vector2 currentLocation);
	bool banPosition(int i);
	size_t searchFreeTiles(Vector2 currentLocation);
	void basicMovement(Vector2 currentLocation);

protected:
	Ghost();

	void persecucion(Vector2 destino, float deltaTime);
	Vector2 destino;
	float timeActual;
	float timeProx;
	float timeUpdate;
	bool movementState;
	int typeOfMovement;
	int timeIndex;
};


#endif
