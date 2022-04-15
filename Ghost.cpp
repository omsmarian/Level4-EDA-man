#include "Ghost.h"

Ghost::Ghost()
{
	this->direction = DirectionUp;
}

void Ghost::update(float deltaTime)
{
	Vector2 increment = { 0,0 };
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
		this->direction = DirectionUp;
}
