#include "Ghost.h"

Ghost::Ghost()
{
	this->direction = DirectionNone;
}

void Ghost::update(float deltaTime)
{
	this->persecucion({ 0,-0.25 }, deltaTime);

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
	//Vector2 flags = { destino.x - this->coordinates.x, destino.y - this->coordinates.y };
	this->destino = destino;
	this->verification(this->coordinates);
	Vector2 increment = { 0,0 };
	this->isMoving = false;

	if (!(this->isMoving) && this->direction == DirectionUp)					//subir			
	{
		printf("up\n");
		increment.y = deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionLeft)			//izquierda
	{
		printf("izq\n");
		increment.x = -deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionRight)
	{
		printf("der\n");
		increment.x = deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionDown)
	{
		printf("abajo\n");
		increment.y = -deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	
}

void Ghost::verification(Vector2 currentLocation)
{
	Setpoint prueba[4] = {{currentLocation.x,currentLocation.y + 0.1,0},		//arriba
				{currentLocation.x - 0.1,currentLocation.y,0},				//izquierda
				{currentLocation.x + 0.1,currentLocation.y,0},					//derecha
				{currentLocation.x,currentLocation.y - 0.1,0}};					//abajo

	float norma = Vector2Distance({ 1.4, 1.7 }, { -1.3, -1.8 });		//the value of the diagonal of the maze in meters
	int proxDirection = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if((this->gameModel->isTileFree(this->getTilePosition(prueba[i]))))
		{
			float newNorm = Vector2Distance(prueba[i].position, this->destino);
			if(norma > newNorm && banPosition(i))
			{
				norma = newNorm;
				proxDirection = i;
			}	
		}
	}
	///////////////////////////
	// TO DO ::: ver de hacer lo de la norma solo para intersecciones
	///////////////////////////
	

	if(proxDirection==0)		//se mueve para arriba
		this->direction = DirectionUp;
	else if(proxDirection==1)		//se mueve para izquierda
		this->direction = DirectionLeft;
	else if(proxDirection==2)		//se mueve para derecha
		this->direction = DirectionRight;
	else if (proxDirection == 3)		//se mueve para abajo
		this->direction = DirectionDown;
	
}

bool Ghost::banPosition(int i){
	if ((this->direction == DirectionUp) && i == DirectionDown)
		return false;
	else if ((this->direction == DirectionDown) && i == DirectionUp)
		return false;
	else if ((this->direction == DirectionRight) && i == DirectionLeft)
		return false;
	else if ((this->direction == DirectionLeft) && i == DirectionRight)
		return false;
	return true;


	/*Direction oppositeDirection = DirectionNone;
	switch (i)
	{
		case (0): oppositeDirection = DirectionDown; break;
		case (1): oppositeDirection = DirectionRight; break;
		case (2): oppositeDirection = DirectionLeft; break;
		case (3): oppositeDirection = DirectionUp; break;
		default: break;
	}
	if(this->direction == oppositeDirection)
		return 1;
	
	return 0;
	*/
}