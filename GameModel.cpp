/**
 * EDA-Man
 *
 * @file GameModel.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include <iostream>
#include <vector>

#include "GameModel.h"

using namespace std;

#define PROXIMITY_CONSTANT  0.18f
const int MAZE_WIDTH = 28;
const int MAZE_HEIGHT = 36;
const int MAZE_SIZE = MAZE_WIDTH * MAZE_HEIGHT;

/**
 * @brief Construct a new Game Model:: Game Model object
 *
 * @param mqttClient An MQTTClient object
 */
GameModel::GameModel(MQTTClient* mqttClient)
{
	this->mqttClient = mqttClient;
	this->gameState = GameStart;
	this->gameStateTime = 0;
	this->liftedRobots = 0;
}

/**
 * @brief Setter for the GameView.
 *
 * @param gameView A GameView object
 */
void GameModel::setGameView(GameView* gameView)
{
	this->gameView = gameView;
}

/**
 * @brief Adds a robot to the game model.
 *
 * @param robot The robot
 */
void GameModel::addRobot(Robot* robot)
{
	robots.push_back(robot);
}

/**
 * @brief Resets game model to initial game conditions.
 *
 * @param maze A maze
 */
void GameModel::start(string maze)
{
	this->maze = maze;
	this->pointsMaze = this->getPointsMaze(maze);
	this->maze.resize(MAZE_SIZE);
	this->score = 0;
	this->energyzerOn = false;

	for (auto c : maze)
	{
		if (c == '+')
			remainingDots++;
		else if (c == '*')
			remainingEnergizers++;
	}

	gameView->start(maze);

	lives = 4;
	eatenFruits.clear();

	gameState = GameStart;

	gameView->setMessage(GameViewMessageReady);
	gameView->setLives(lives);
	gameView->setEatenFruits(eatenFruits);


	gameView->playAudio("mainStart");
	for (auto robot : robots)
		robot->start(this->mqttClient, this);

	this->gameState = GameStarting;
}
/*@brief Duplicates maze to view only dots and energizers
*
*@param original maze
*
* @return new maze
*/
std::string GameModel::getPointsMaze(std::string originalMaze)
{
	std::string pointsMaze = originalMaze;
	char* placePointer;
	for (int i = 0; i < pointsMaze.size(); i++)
	{
		placePointer = &pointsMaze[i];

		if (*placePointer == '+')		//finds a point
			*placePointer = '1';
		else if (*placePointer == '#')		//finds an energizer
			*placePointer = '2';
		else
			*placePointer = '0';
	}
	return pointsMaze;
}

/**
 * @brief Updates game model for current frame.
 *
 */
void GameModel::update(float deltaTime)
{
	this->gameStateTime += deltaTime;

	if (gameStateTime >= 1.0)	//take away ready message
	{
		this->gameView->setMessage(GameViewMessageNone);
	}

	int x, y;



	Setpoint playerSetPoint = { robots[0]->getCoordinates(),0 };						//define setpoint and coordinates for player
	Vector2 playerPosition = robots[0]->getTilePosition(playerSetPoint);
	playerPosition.y -= 0.05;

	char* tile = &(this->pointsMaze[MAZE_WIDTH *
		((int)playerPosition.y) + ((int)playerPosition.x)]);		//get tile below player

	if (*tile != '0')							//check for dot or energizer
	{
		this->gameView->clearTile((int)playerPosition.x, (int)playerPosition.y);
		static int wakaWaka = 1;
		this->robots[0]->setImageIndex(robots[0]->getOriginalImage() + wakaWaka);
		wakaWaka = -wakaWaka;
		if (*tile == '1')						//finds a dot
		{
			this->remainingDots--;
			this->score += 10;
		}
		else if (*tile == '2')					//finds a energizer
		{
			this->remainingEnergizers--;
			this->score += 50;
			this->energyzerOn = true;
			energizerTime = gameStateTime;
		}
		*tile = '0';
	}
	if (energyzerOn)								//if pacman eats an energizer
	{
		static bool image = true;
		if (image)
		{
			for (int i = 1; i < robots.size(); i++)						//change ghost image
			{
				robots[i]->setImageIndex(25);
			}
			image = false;
		}
		if (this->gameStateTime >= (this->energizerTime + 7.0))				//set energyzer time limit
		{
			this->energyzerOn = false;
			image = true;
			for (int i = 1; i < robots.size(); i++)
			{
				robots[i]->setImageIndex(robots[i]->getOriginalImage());
			}
		}
	}
	Vector2 playerCoords = this->robots[0]->getCoordinates();
	if ((playerCoords.x > 1.2) && (playerCoords.y > -0.1) && (playerCoords.y < 0.1))
	{
		this->robots[0]->liftTo({ -1.2,0,0 });
		this->robots[0]->setCoordinates({ -1.1,0 });
		this->robots[0]->setSetpoint({ {-1.1,0},0 });
	}
	else if ((playerCoords.x < -1.2) && (playerCoords.y > -0.1) && (playerCoords.y < 0.1))
	{
		this->robots[0]->liftTo({ 1.2,0,0 });
		this->robots[0]->setCoordinates({ 1.1,0 });
		this->robots[0]->setSetpoint({ {1.1,0},0 });
	}

	bool colision = viewColision();				//check for colision
	if (colision)
	{
		int quantityOfEatenGhosts = 0;
		if (this->energyzerOn)					//player chases ghosts
		{
			quantityOfEatenGhosts++;
			this->score += 200 * quantityOfEatenGhosts;
			this->robots[robotToReset]->resetRobot();
			for (int i = 1; i < robots.size(); i++)
			{

				robots[i]->setSetpoint({ robots[i]->getCoordinates(), 0 });
			}
			this->liftedRobots++;
		}
		else							//ghost colide player
		{
			this->lives--;
			resetGame();
			this->liftedRobots = 0;
			this->gameView->setMessage(GameViewMessageReady);
			delay(1);
			this->gameView->setMessage(GameViewMessageNone);
		}
		colision = false;
	}
	for (auto robot : robots)		//update robots position
		robot->update(deltaTime);

	this->gameView->setScore(this->score);
	this->gameView->setLives(this->lives);
	if (this->lives == 0)		//player out of lives
	{

		this->gameView->setMessage(GameViewMessageGameOver);
		this->gameState = GameEnding;
	}
	this->gameView->update(GameViewMessageNone);
}


/**
 * @brief Determine if a tile is free.
 *
 * @param tilePosition A tile coordinate
 * @return true Tile is free
 * @return false Tile is not free
 */
bool GameModel::isTileFree(Vector2 tilePosition)
{
	if ((tilePosition.x < 0) || (tilePosition.x >= MAZE_WIDTH))
		return false;

	if ((tilePosition.y < 0) || (tilePosition.y >= MAZE_HEIGHT))
		return false;

	char tile = this->maze[(int)tilePosition.y * MAZE_WIDTH + (int)tilePosition.x];

	return (tile == ' ') || (tile == '+') || (tile == '#');
}
/**
 * @brief gets position of robot
 *
 * @param robot number
 *
 * @return coordinates of robot
 */

Vector2 GameModel::getPosition(int i)
{
	return this->robots[i]->getCoordinates();
}
/**
 * @brief gets direction of robot
 *
 * @param robot number
 *
 * @return direction of robot
 */
int GameModel::getPlayerDirection(int i)
{
	return this->robots[i]->getDirection();
}
/**
 * @brief checks for colision
 *
 * @return colision flag
 */
bool GameModel::viewColision()
{
	Vector2 playerPosition = getPosition(0);

	for (int i = 1; i < robots.size(); i++)		//Checks distance between robots
	{
		Vector2 ghostPosition = getPosition(i);
		float distance = Vector2Distance(playerPosition, ghostPosition);
		if (distance <= PROXIMITY_CONSTANT)
		{
			robotToReset = i;
			return true;
		}
	}
	return false;
}
/**
 * @brief resets all robots to initial positions
 *
 */
void GameModel::resetGame()
{
	for (auto robot : robots)
	{
		robot->resetRobot();
	}
}
/**
 * @brief makes a delay in program
 *
 * @param number of seconds to delay
 */
void GameModel::delay(int numberOfSeconds)
{
	// Converting time into milli_seconds
	int milliSeconds = 1000 * numberOfSeconds;

	// Storing start time
	clock_t startTime = clock();

	// looping till required time is not achieved
	while (clock() < startTime + milliSeconds)
	{
	}
}