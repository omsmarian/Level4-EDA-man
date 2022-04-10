#ifndef _PLAYER_H
#define _PLAYER_H

#include "Robot.h"

/*
Clase del jugador heredada de robot 
*/
class Player : public Robot
{
public:

    RobotSetpoint position;
    void start(MQTTClient* client , GameModel * model);
    void movement(int posX, int posZ);
    void reset();

private:
    MazePosition coordenates;

};


#endif
