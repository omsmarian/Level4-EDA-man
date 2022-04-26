/**
 * EDA-Man
 *
 * @file GameView.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#include <cmath>
#include <cstring>  // std::memcpy
#include <iomanip>
#include <sstream>

#include "GameView.h"

using namespace std;

const int MAZE_WIDTH = 28;
const int MAZE_HEIGHT = 36;
const int MAZE_SIZE = MAZE_WIDTH * MAZE_HEIGHT;

const int SCORE_COLOR = 15;
const int LAYOUT_COLOR = 11;
const int LAYOUT_DOOR_COLOR = 3;
const int DOTS_COLOR = 14;

const int GAMEOVER_COLOR = 1;
const int READY_COLOR = 9;

/**
 * @brief Construct a new Game View:: Game View object
 *
 * @param mqttClient An MQTT client instance
 */
GameView::GameView(MQTTClient* mqttClient)
{
	this->mqttClient = mqttClient;

	time = 0;

	blink1Up = false;
	blinkEnergizers = true;

	gameViewMessage = GameViewMessageNone;
}

/**
 * @brief Resets screen to initial game conditions.
 *
 * @param maze A maze (size 28x36)
 */
void GameView::start(string maze)
{
	maze.resize(MAZE_SIZE);

	// Draw layout
	string mazeLayout;
	for (int i = 0; i < maze.size(); i++)
	{
		if ((maze[i] == '+') || (maze[i] == '#') || (maze[i] == '_'))
			mazeLayout += ' ';
		else
			mazeLayout += maze[i];
	}
	setTiles(0, 0, LAYOUT_COLOR, mazeLayout);

	// Draw dots and doors
	for (int i = 0; i < maze.size(); i++)
	{
		int x = i % MAZE_WIDTH;
		int y = i / MAZE_WIDTH;

		if (maze[i] == '+')
			setTiles(x, y, DOTS_COLOR, "+");
		else if (maze[i] == '#')
		{
			setTiles(x, y, DOTS_COLOR, "#");

			Energizer energizer;
			energizer.x = x;
			energizer.y = y;
			energizers.push_back(energizer);
		}
		else if (maze[i] == '_')
			setTiles(x, y, LAYOUT_DOOR_COLOR, "_");
	}

	setTiles(0, 0, SCORE_COLOR, "         HIGH SCORE         ");
	setTiles(0, 1, SCORE_COLOR, "     00                     ");
	setTiles(0, 2, SCORE_COLOR, "                            ");

	setTiles(0, 34, SCORE_COLOR, "                            ");
	setTiles(0, 35, SCORE_COLOR, "                            ");
}

/**
 * @brief Updates screen for current frame.
 *
 * @param deltaTime Number of seconds since the last frame.
 */
void GameView::update(float deltaTime)
{
	time += deltaTime;

	float blink1UpPhase = (float)fmod(time / (32.0 / 60.0), 1.0);
	bool nextBlink1Up = (blink1UpPhase > 0.5);
	if (blink1Up != nextBlink1Up)
	{
		blink1Up = nextBlink1Up;

		setTiles(3, 0, blink1Up ? SCORE_COLOR : 0, "1UP");
	}

	if (gameViewMessage == GameViewMessageNone)
	{
		float blinkPowerUpsPhase = (float)fmod(time / (20.0 / 60.0), 1.0);
		bool nextBlinkEnergizers = (blinkPowerUpsPhase > 0.5);
		if (blinkEnergizers != nextBlinkEnergizers)
		{
			blinkEnergizers = nextBlinkEnergizers;

			for (auto energizer : energizers)
				setTiles(energizer.x, energizer.y, blink1Up ? DOTS_COLOR : 0, "#");
		}
	}
}

/**
 * @brief Sets game message ("READY!" at beginning of level, "GAME  OVER" at end of game, or hides message).
 *
 * @param value A GameViewMessage
 */
void GameView::setMessage(GameViewMessage value)
{
	if (value == gameViewMessage)
		return;

	gameViewMessage = value;

	if (gameViewMessage == GameViewMessageReady)
		setTiles(9, 20, READY_COLOR, "  READY!  ");
	else if (gameViewMessage == GameViewMessageGameOver)
		setTiles(9, 20, GAMEOVER_COLOR, "GAME  OVER");
	else if (gameViewMessage == GameViewMessagePlayAgain)
		setTiles(2, 34, GAMEOVER_COLOR, "PRESS SPACE TO PLAY AGAIN");
	else
		setTiles(9, 20, 0, "          ");
}

/**
 * @brief Clears a tile (dot, energizer or fruit).
 *
 * @param x x tile coordinate
 * @param y y tile coordinate
 */
void GameView::clearTile(int x, int y)
{
	setTiles(x, y, 0, " ");

	// Remove from energizers
	list<Energizer> newEnergizers;
	for (auto energizer : energizers)
	{
		if (!((energizer.x == x) && (energizer.y == y)))
			newEnergizers.push_back(energizer);
	}
	energizers = newEnergizers;
}

/**
 * @brief Places a fruit in the maze.
 *
 * @param x x tile coordinate of fruit
 * @param y y tile coorindate of fruit
 * @param fruitIndex Type of fruit (0 to 7)
 */
void GameView::setFruit(int x, int y, int fruitIndex)
{
	string s = " ";
	s[0] = 0xc0 + fruitIndex;
	setTiles(x, y, 15, s);
}

/**
 * @brief Sets score.
 *
 * @param value The score
 */
void GameView::setScore(int value)
{
	ostringstream ss;
	ss << right << setw(7) << value;
	setTiles(0, 1, SCORE_COLOR, ss.str());
}

/**
 * @brief Sets high score.
 *
 * @param value The high score
 */
void GameView::setHighScore(int value)
{
	ostringstream ss;
	ss << right << setw(7) << value;
	setTiles(11, 1, SCORE_COLOR, ss.str());
}

/**
 * @brief Updates number of lives left indicator (lower left).
 *
 * @param numberOfLives Number of lives left
 */
void GameView::setLives(int numberOfLives)
{
	const int maxLives = 5;

	if (numberOfLives < 0)
		numberOfLives = 0;
	if (numberOfLives > maxLives)
		numberOfLives = maxLives;

	string line1(2 * maxLives, ' ');
	string line2(2 * maxLives, ' ');

	for (int i = 0; i < numberOfLives; i++)
	{
		line1[2 * i] = (char)0xa0;
		line1[2 * i + 1] = (char)0xa1;
		line2[2 * i] = (char)0xb0;
		line2[2 * i + 1] = (char)0xb1;
	}

	setTiles(2, 34, SCORE_COLOR, line1);
	setTiles(2, 35, SCORE_COLOR, line2);
}

/**
 * @brief Updates the eaten fruits indicator (lower right).
 *
 * @param fruitIndices List of all eaten fruits (fruitIndex is type of fruit: 0 to 7)
 */
void GameView::setEatenFruits(list<int>& fruitIndices)
{
	const int maxFruits = 7;

	string line1(2 * maxFruits, ' ');
	string line2(2 * maxFruits, ' ');

	int n = (int)fruitIndices.size();
	if (n > maxFruits)
		n = maxFruits;

	auto fruitIndexElement = fruitIndices.end();
	for (int i = (n - 1); i >= 0; i--)
	{
		int lineIndex = 2 * (maxFruits - 1 - i);

		fruitIndexElement--;
		int fruitIndex = (*fruitIndexElement) & 0x7;

		line1[lineIndex + 0] = 0x80 + 2 * fruitIndex;
		line1[lineIndex + 1] = 0x81 + 2 * fruitIndex;
		line2[lineIndex + 0] = 0x90 + 2 * fruitIndex;
		line2[lineIndex + 1] = 0x91 + 2 * fruitIndex;
	}

	setTiles(12, 34, SCORE_COLOR, line1);
	setTiles(12, 35, SCORE_COLOR, line2);
}

/**
 * @brief Sends screen tiles. Wraps long lines to next row(s).
 *
 * @param x start in x tile coordinates
 * @param y start in y tile coordinates
 * @param palette Palette color (tints characters 0x00-0x7f)
 * @param s Tile data
 */
void GameView::setTiles(int x, int y, int palette, string s)
{
	vector<char> payload(3 + s.size());

	payload[0] = x;
	payload[1] = y;
	payload[2] = palette;
	memcpy(payload.data() + 3, s.c_str(), s.size());

	mqttClient->publish("ledfloor/tiles/set", payload);
}

/**
 * @brief Clears the LED floor.
 *
 */
void GameView::clearScreen()
{
	setTiles(0, 0, 0, string(MAZE_SIZE, ' '));
}

/**
 * @brief Plays a sound.
 *
 * @param audioId Audio id
 */
void GameView::playAudio(string audioId)
{
	vector<char> payload;

	mqttClient->publish("jukebox/" + audioId + "/play", payload);
}

/**
 * @brief Stops a sound.
 *
 * @param audioId Audio id
 */
void GameView::stopAudio(string audioId)
{
	vector<char> payload;

	mqttClient->publish("jukebox/" + audioId + "/stop", payload);
}
