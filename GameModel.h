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

    int viewColision();

    void resetGame();
    void stopGame();

    GameState gameState;

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

    int remainingDots;
    int remainingEnergizers;

    int lives;
    std::list<int> eatenFruits;
};


#endif
