#ifndef GHOST_H
#define GHOST_H

#include "Robot.h"
#include "raymath.h"

#define VELOCIDAD_GHOST 0.6f

class Ghost : public Robot
{
public:

private:
	void verification(Vector2 currentLocation);
	bool banPosition(int i);
	size_t searchFreeTiles(Vector2 currentLocation);
	void movimientoMono(Vector2 currentLocation);

protected:
	Ghost();
	
	void persecucion(Vector2 destino, float deltaTime);
	Vector2 destino;
	float timeActual;
	float timeProx;
	float timeUpdate;
	bool movementState;
};


#endif
