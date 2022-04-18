#include "Ghost.h"

Ghost::Ghost()
{
	this->direction = DirectionUp;
}

void Ghost::update(float deltaTime)
{
	this->persecucion({ 0,-0.85 }, deltaTime);

	/*Vector2 increment = { 0,0 };
	if (this->direction == DirectionUp)
	{
		increment.y = deltaTime * VELOCIDAD;
		this->setpoint.rotation = 0;
	}
	else if (this->direction == DirectionDown)
	{
		increment.y = -deltaTime * VELOCIDAD;
		this->setpoint.rotation = 180;
	}
	else if (this->direction == DirectionRight)
	{
		increment.x = deltaTime * VELOCIDAD;
		this->setpoint.rotation = 90;
	}
	else if (this->direction == DirectionLeft)
	{
		increment.x = -1 * deltaTime * VELOCIDAD;
		this->setpoint.rotation = 270;
	}
	this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

	if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
		this->movement(increment);

	else if (this->direction == DirectionUp)
		this->direction = DirectionLeft;

	else if (this->direction == DirectionLeft)
		this->direction = DirectionRight;

	else if (this->direction == DirectionRight)
		this->direction = DirectionDown;

	else
		this->direction = DirectionUp;*/
}



void Ghost::persecucion(Vector2 destino, float deltaTime)	// arriba, izq, derecha, abajo
{
	/*
	Q NO SE PUEDA VOLVER PARA ATRAS
	Q CALCULE LA DISTANCIA MÁS CORTA EN LOS BLOQUES Q PUEDE MOVERSE
	*/
	Vector2 flags = { destino.x - this->coordinates.x, destino.y - this->coordinates.y };

	Vector2 increment = { 0,0 };
	this->isMoving = false;

	if ((this->isMoving == false) && (flags.y > 0))					//subir			
	{
		printf("up\n");
		increment.y = deltaTime * VELOCIDAD;
		this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
			this->movement(increment);
	}
	increment = { 0,0 };
	if ((this->isMoving == false) && (flags.x < 0))			//izquierda
	{
		printf("izq\n");
		increment.x = -deltaTime * VELOCIDAD;
		this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
			this->movement(increment);
	}
	increment = { 0,0 };
	if ((this->isMoving == false) && (flags.x > 0))
	{
		printf("der\n");
		increment.x = deltaTime * VELOCIDAD;
		this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
			this->movement(increment);
	}
	increment = { 0,0 };
	if ((this->isMoving == false) && (flags.y < 0))
	{
		printf("abajo\n");
		increment.y = -deltaTime * VELOCIDAD;
		this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
			this->movement(increment);
	}
		//if ((!this->isMoving) && flagy)					// SI ES 0 NO ENTRA
		//{
		//	if (flagy < 0)			//BAJAR
		//	{
		//		increment.y = -deltaTime * VELOCIDAD;
		//	}
		//	else if (flagy > 0)		//SUBIR
		//	{
		//		increment.y = deltaTime * VELOCIDAD;
		//	}
		//	this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		//	if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
		//		this->movement(increment);
		//}

		//increment = {0,0};

		//if ((!this->isMoving) && flagx)			// SE MUEVE? Y VALE LA PENA MOVERSE EN X
		//{
		//	if (flagx < 0)			//IZQUIERDA
		//	{
		//		increment.x = -deltaTime * VELOCIDAD;
		//	}
		//	else if (flagx > 0)		//DERECHA
		//	{
		//		increment.x = deltaTime * VELOCIDAD;
		//	}

		//	this->setpoint.position = { this->coordinates.x + increment.x, this->coordinates.y + increment.y };

		//	if (this->gameModel->isTileFree(this->getTilePosition(this->setpoint)))
		//		this->movement(increment);
		//}
}
