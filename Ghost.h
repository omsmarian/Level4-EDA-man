#ifndef GHOST_H
#define GHOST_H

#include "Robot.h"

#define VELOCIDAD 0.6f

class Ghost : public Robot
{
public:
	void update(float deltaTime);

protected:
	Ghost();
};


#endif
