/**
 * EDA-Man
 *
 * @file GameModel.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

class Robot;

/**
 * @brief Game state
 *
 */
enum GameState
{
	GameStart,
	GameStarting,
	GamePlaying,
	GameEnding
};

#include <string>
#include <list>
#include <vector>

#include <raylib.h>
#include "raymath.h"

#include "MQTTClient.h"

#include "GameView.h"
#include "Robot.h"

class GameModel
{
public:
	GameModel(MQTTClient* mqttClient);

	void setGameView(GameView* gameView);

	void addRobot(Robot* robot);

	void start(std::string maze);
	void update(float deltaTime);

	bool isTileFree(Vector2 position);

	Vector2 getPosition(int i);
	int getPlayerDirection(int i);

	bool viewColision();

	void resetGame();
	void delay(int numberOfSeconds);

	GameState gameState;
	int liftedRobots;

private:
	MQTTClient* mqttClient;
	GameView* gameView;

	std::string getPointsMaze(std::string);

	std::string maze;
	std::string pointsMaze;
	std::vector<Robot*> robots;


	float gameStateTime;

	int score;

	bool energyzerOn;
	float energizerTime;
	int robotToReset;

	int remainingDots;
	int remainingEnergizers;

	int lives;
	std::list<int> eatenFruits;
};


#endif
