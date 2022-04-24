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
    GamePlaying
};

#include <string>
#include <list>
#include <vector>

#include <raylib.h>

#include "MQTTClient.h"

#include "GameView.h"
#include "Robot.h"

class GameModel
{
public:
    GameModel(MQTTClient *mqttClient);

    void setGameView(GameView *gameView);

    void addRobot(Robot *robot);

    void start(std::string maze);
    void update(float deltaTime);

    bool isTileFree(Vector2 position);

    Vector2 getPosition(int i);
    int getPlayerDirection(int i);

private:
    MQTTClient *mqttClient;
    GameView *gameView;

    std::string maze;
    std::vector<Robot *> robots;

    int gameState;
    float gameStateTime;

    int remainingDots;
    int remainingEnergizers;

    int lives;
    std::list<int> eatenFruits;
};

#endif
