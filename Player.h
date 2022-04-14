#ifndef PLAYER_H
#define PLAYER_H

#include "Robot.h"


class Player : public Robot
{
public:
	Player(std::string id, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
private:

};



#endif // !PLAYER_H