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


	for (auto robot : robots)
		robot->start(this->mqttClient, this);

	// Just for testing
	gameView->playAudio("mainStart");
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

	int x, y;

	for (auto robot : robots)
		robot->update(deltaTime);


	Vector2 playerPosition = robots[0]->getCoordinates();
	//printf("x = %f , y = %f\n", playerPosition.x, playerPosition.y);

	char* tile = &(this->pointsMaze[-1 * ((int)(playerPosition.y * 10) - 18) * MAZE_WIDTH + ( - 1 * ((int)(playerPosition.x * 10) - 14))]);

	if (*tile != '0')			//finds a point
	{
		printf("x = %f , y = %f\n", playerPosition.x, playerPosition.y);
		this->gameView->clearTile(27 - ( - 1 * ((int)(playerPosition.x * 10) - 14)), -1 * ((int)(playerPosition.y * 10) - 18));
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
		}
		*tile = '0';
	}
	if (energyzerOn)
	{
		//    frightened();

	}
	if (viewColision())
	{
		this->lives--;
		resetGame();

	}


	this->gameView->setScore(this->score);
	this->gameView->setLives(this->lives);

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

	char tile = maze[(int)tilePosition.y * MAZE_WIDTH + (int)tilePosition.x];

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

bool GameModel::viewColision()
{
	Vector2 playerPosition = getPosition(0);

	for (int i = 1; i < robots.size(); i++)
	{
		Vector2 ghostPosition = getPosition(i);
		float distance = Vector2Distance(playerPosition, ghostPosition);
		if (distance <= PROXIMITY_CONSTANT)
		{
			return true;
		}
	}

	return false;
}

void GameModel::resetGame()
{
	for (auto robot : robots)
	{
		robot->resetRobot();
	}
}