/**
 * EDA-Man
 *
 * @file GameView.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef _GAMEVIEW_H
#define _GAMEVIEW_H

#include <list>
#include <string>
#include <vector>

#include "MQTTClient.h"

 /**
  * @brief A message for the GameView setMessage method.
  *
  */
enum GameViewMessage
{
	GameViewMessageNone,
	GameViewMessageReady,
	GameViewMessageGameOver,
	GameViewMessagePlayAgain,
};

class GameView
{
public:
	GameView(MQTTClient* mqttClient);

	void start(std::string maze);
	void update(float deltaTime);

	void setMessage(GameViewMessage value);

	void clearTile(int x, int y);
	void setFruit(int x, int y, int fruitIndex);

	void setScore(int value);
	void setHighScore(int value);
	void setLives(int numberOfLives);
	void setEatenFruits(std::list<int>& eatenFruits);

	void setTiles(int x, int y, int palette, std::string s);
	void clearScreen();

	void playAudio(std::string audioId);
	void stopAudio(std::string audioId);

private:
	struct Energizer
	{
		int x;
		int y;
	};

	MQTTClient* mqttClient;

	float time;

	bool blink1Up;
	bool blinkEnergizers;
	std::list<Energizer> energizers;

	GameViewMessage gameViewMessage;
};

#endif
