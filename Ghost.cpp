#include "Ghost.h"

Ghost::Ghost()
{
	this->timeActual = 0;
	this->timeProx = 0;
	this->direction = DirectionNone;
}

void Ghost::update(float deltaTime)
{
	this->persecucion({1.25,1.35}, deltaTime);

}



void Ghost::persecucion(Vector2 destino, float deltaTime)	// arriba, izq, derecha, abajo
{
	this->destino = destino;
	this->timeActual += deltaTime;

	if(timeActual >= timeProx)
	{
		this->verification(this->coordinates);
		timeProx = timeActual + 0.1/VELOCIDAD;
	}

	Vector2 increment = { 0,0 };
	this->isMoving = false;
	if (!(this->isMoving) && this->direction == DirectionUp)	//subir			
	{
		printf("up\n");
		increment.y = deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionLeft)	//izquierda
	{
		printf("izq\n");
		increment.x = -deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionRight)	//derecha
	{
		printf("der\n");
		increment.x = deltaTime * VELOCIDAD;
		this->movement(increment);
	}
	increment = { 0,0 };
	if (!(this->isMoving) && this->direction == DirectionDown)	//abajo
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

}