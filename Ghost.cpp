/**
 * EDA-Man
 *
 * @file Ghost.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include "Ghost.h"
 /**
   * @brief ghost inicialization
   *
   */
Ghost::Ghost()
{
	this->timeActual = 0;
	this->timeProx = 0;
	this->timeUpdate = 0;
	this->direction = DirectionNone;
	this->setpoint.rotation = 0;
	this->timeIndex = 0;
}
/**
  * @brief Ghost persecution logic
  *
  * @param destiny of robot and deltaTime
  */
void Ghost::persecucion(Vector2 destino, float deltaTime)
{
	this->destino = destino;
	this->timeActual += deltaTime;
	size_t freeTiles = this->searchFreeTiles(this->coordinates);

	if (timeActual >= timeProx)
	{
		if (freeTiles > 2)
			this->verification(this->coordinates);
		else if (freeTiles <= 2)
		{
			this->basicMovement(this->coordinates);
		}
		timeProx = timeActual + 0.1 / VELOCIDAD_GHOST;
	}

	Vector2 increment = { 0,0 };
	this->isMoving = false;
	if (!(this->isMoving) && this->direction == DirectionUp)	//subir			
	{
		increment.y = deltaTime * VELOCIDAD_GHOST;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionLeft)	//izquierda
	{
		increment.x = -deltaTime * VELOCIDAD_GHOST;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionRight)	//derecha
	{
		increment.x = deltaTime * VELOCIDAD_GHOST;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionDown)	//abajo
	{
		increment.y = -deltaTime * VELOCIDAD_GHOST;
		this->movement(increment);
	}
}
/**
  * @brief makes route for ghost to get to another position
  *
  * @param  ghost current location
  */
void Ghost::verification(Vector2 currentLocation)
{
	Setpoint prueba[4] = { {currentLocation.x,currentLocation.y + 0.1,0},		//arriba
				{currentLocation.x - 0.1,currentLocation.y,0},				//izquierda
				{currentLocation.x + 0.1,currentLocation.y,0},					//derecha
				{currentLocation.x,currentLocation.y - 0.1,0} };					//abajo

	float norma = Vector2Distance({ 1.4, 1.7 }, { -1.3, -1.8 });		//the value of the diagonal of the maze in meters
	int proxDirection = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (this->gameModel->isTileFree(this->getTilePosition(prueba[i])))
		{
			float newNorm = Vector2Distance(prueba[i].position, this->destino);
			if (norma > newNorm && banPosition(i))
			{
				norma = newNorm;
				proxDirection = i;
			}
		}
	}

	if (proxDirection == 0)		//se mueve para arriba
		this->direction = DirectionUp;
	else if (proxDirection == 1)		//se mueve para izquierda
		this->direction = DirectionLeft;
	else if (proxDirection == 2)		//se mueve para derecha
		this->direction = DirectionRight;
	else if (proxDirection == 3)		//se mueve para abajo
		this->direction = DirectionDown;

}

bool Ghost::banPosition(int i) {
	if ((this->direction == DirectionUp) && i == DirectionDown)
		return false;
	else if ((this->direction == DirectionDown) && i == DirectionUp)
		return false;
	else if ((this->direction == DirectionRight) && i == DirectionLeft)
		return false;
	else if ((this->direction == DirectionLeft) && i == DirectionRight)
		return false;
	return true;

}

size_t Ghost::searchFreeTiles(Vector2 currentLocation)
{
	Setpoint prueba[4] = { {currentLocation.x,currentLocation.y + 0.1,0},		//arriba
				{currentLocation.x - 0.1,currentLocation.y,0},				//izquierda
				{currentLocation.x + 0.1,currentLocation.y,0},					//derecha
				{currentLocation.x,currentLocation.y - 0.1,0} };					//abajo
	size_t num = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if ((this->gameModel->isTileFree(this->getTilePosition(prueba[i]))))
			num++;
	}
	return num;
}

void Ghost::basicMovement(Vector2 currentLocation)
{
	Setpoint prueba[4] = { {currentLocation.x,currentLocation.y + 0.1,0},		//arriba
			   {currentLocation.x - 0.1,currentLocation.y,0},				//izquierda
			   {currentLocation.x + 0.1,currentLocation.y,0},					//derecha
			   {currentLocation.x,currentLocation.y - 0.1,0} };					//abajo

	if ((this->gameModel->isTileFree(this->getTilePosition(prueba[0])))
		&& banPosition(DirectionUp))
	{
		this->direction = DirectionUp;
	}
	else if ((this->gameModel->isTileFree(this->getTilePosition(prueba[1])))
		&& banPosition(DirectionLeft))
	{
		this->direction = DirectionLeft;
	}
	else if ((this->gameModel->isTileFree(this->getTilePosition(prueba[2])))
		&& banPosition(DirectionRight))
	{
		this->direction = DirectionRight;
	}
	else if ((this->gameModel->isTileFree(this->getTilePosition(prueba[3])))
		&& banPosition(DirectionDown))
	{
		this->direction = DirectionDown;
	}
}
