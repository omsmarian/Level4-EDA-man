#ifndef PLAYER_H
#define PLAYER_H

#include "Robot.h"

#define VELOCIDAD 0.64f

class Player : public Robot
{
public:
	Player(std::string id, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);

private:
};



#endif // !PLAYER_H