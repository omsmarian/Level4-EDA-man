#ifndef GHOST_H
#define GHOST_H

#include "Robot.h"
#include "raymath.h"

#define VELOCIDAD 0.6f

class Ghost : public Robot
{
public:
	void update(float deltaTime);

private:
	void persecucion(Vector2 destino, float deltaTime);
	void verification(Vector2 currentLocation);
	bool banPosition(int i);

protected:
	Ghost();
	Vector2 destino;
};


#endif
