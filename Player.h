#ifndef _PLAYER_H
#define _PLAYER_H

#include "Robot.h"

/*
Clase del jugador heredada de robot 
*/
class Player : public Robot
{
public:
    Player(Vector2 origenCoords, std::string robotLabel, int imagePointer);
    Setpoint position;
    void reset();
    int IsRobotInPlace();

private:
};


#endif
