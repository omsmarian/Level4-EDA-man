/**
 * @file GameModel.h
 * @author Marc S. Ressl
 * @brief EDA-Man game model
 * @version 0.1
 * @date 2022-04-12
 *
 * @copyright Copyright (c) 2022
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
/*Duplica el laberinto para conseguir los puntos
*
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
 * @param deltaTime Number of seconds since the last frame
 */
void GameModel::update(float deltaTime)
{
	gameStateTime += deltaTime;

	if (gameStateTime >= 1.0)
	{
		this->gameView->setMessage(GameViewMessageNone);
	}

	int x, y;

	for (auto robot : robots)
		robot->update(deltaTime);

	Setpoint playerSetPoint = { robots[0]->getCoordinates(),0 };
	Vector2 playerPosition = robots[0]->getTilePosition(playerSetPoint);
	playerPosition.y -= 0.05;

	char* tile = &(this->pointsMaze[MAZE_WIDTH * ((int)playerPosition.y) + ((int)playerPosition.x)]);

	if (*tile != '0')           //finds a point
	{    
		this->gameView->clearTile((int)playerPosition.x, (int)playerPosition.y);
		if (*tile == '1')
		{
			this->remainingDots--;
			this->score += 10;
		}
		else if (*tile == '2')
		{
			this->remainingEnergizers--;
			this->score += 50;
			this->energyzerOn = true;
			this->energizerTime = this->gameStateTime;
		}
		*tile = '0';
	}
	if (energyzerOn)
	{
		if (this->gameStateTime >= (this->energizerTime + 7.0))
		{
			this->energyzerOn = false;
		}
	}

	int robotColision = viewColision();
	if (robotColision)
	{
		static int quantityOfEatenGhosts;
		if (this->energyzerOn) //player chases ghosts
		{
			quantityOfEatenGhosts++;
			this->score += 200 * quantityOfEatenGhosts;
			this->robots[robotColision]->resetRobot();
		}
		else
		{
			quantityOfEatenGhosts = 0;
			this->lives--;
			resetGame();
			this->gameView->setMessage(GameViewMessageReady);
			delay(1);
			this->gameView->setMessage(GameViewMessageNone);
		}
	}

	this->gameView->setScore(this->score);
	this->gameView->setLives(this->lives);
	if (this->lives == 0)
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


Vector2 GameModel::getPosition(int i)
{
	return this->robots[i]->getCoordinates();
}

int GameModel::getPlayerDirection(int i)
{
	return this->robots[i]->getDirection();
}

int GameModel::viewColision()
{
	for (int i = 0; i < robots.size(); i++)
	{
		Vector2 robot1Position = getPosition(i);
		for(int j = 0; j < robots.size(); j++)
		{
			if(j != i)
			{
				Vector2 robot2Position = getPosition(j);
				float distance = Vector2Distance(robot1Position, robot2Position);
				if (distance <= PROXIMITY_CONSTANT)
				{
					if(i == 0 || j == 0)
					{
						return j;
					}
					else
					{
						for(int k = 0; k<2; k++)
						{
							k = k ? i : j;
							Direction direccion = this->robots[i]->getDirection();
							if(direccion == DirectionUp)
								this->robots[i]->setDirection(DirectionDown);
							else if(direccion == DirectionDown)
								this->robots[i]->setDirection(DirectionUp);
							else if(direccion == DirectionLeft)
								this->robots[i]->setDirection(DirectionRight);
							else if(direccion == DirectionRight)
								this->robots[i]->setDirection(DirectionLeft);
						}
					}					
				}
			}
		}
	}
	return 0;
}

void GameModel::resetGame()
{
	for (auto robot : robots)
	{
		robot->resetRobot();
	}
}

void GameModel::delay(int numberOfSeconds)
{
	// Converting time into milli_seconds
	int milliSeconds = 1000 * numberOfSeconds;

	// Storing start time
	clock_t startTime = clock();

	// looping till required time is not achieved
	while (clock() < startTime + milliSeconds)
		;
}