#ifndef GHOST_H
#define GHOST_H

#include "Robot.h"

#define VELOCIDAD 0.6f

class Ghost : public Robot
{
public:
	void update(float deltaTime);

private:
	void persecucion(Vector2 destino, float deltaTime);

protected:
	Ghost();
	Vector2 destino;
};


#endif
