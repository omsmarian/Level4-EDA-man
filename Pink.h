#ifndef PINK_H
#define PINK_H

#include "Ghost.h"

class Pink : public Ghost
{
public:
	Pink(std::string robotId, Vector2 spawn);
	void start(MQTTClient* client, GameModel* model);
	void update(float deltaTime);
	Vector2 getDestinationPink();

private:

	int lastDirection;
};

#endif // !PINK_H