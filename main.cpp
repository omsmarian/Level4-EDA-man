/**
 * EDA-Man
 *
 * @copyright Copyright (C) 2022
 * @author Marc S. Ressl
 * 
 * @brief Controls an EDA-Man game.
 */

#include <iostream>

#include <raylib.h>

#include "MQTTClient.h"

#include "GameModel.h"
#include "GameView.h"
#include "Player.h"
#include "Red.h"
#include "Pink.h"
#include "Cyan.h"
#include "Orange.h"

using namespace std;

/*
* Updates the direction of the player
* given by the input state
*/
void updateInput (Player* player);

int main(int, char **)
{
    MQTTClient mqttClient("controller");

    const int port = 1883;
    if (!mqttClient.connect("localhost", port, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Could not connect." << endl;
        return 1;
    }

    cout << "Connected." << endl;

    // raylib
    InitWindow(640, 480, "EDAPark Controller");
    SetTargetFPS(60);

    string maze =
        "                            "      //0,1.7             (int)(coordenadas.y * 10) -> 0 
        "                            "      //0,1.6
        "                            "      //0,1.5
        "jbbbbbbbbbbbbonbbbbbbbbbbbbk"      //0,1.4
        "s++++++++++++pq++++++++++++r"      //0,1.3
        "s+faag+faaag+pq+faaag+faag+r"      //0,1.2
        "s#p  q+p   q+pq+p   q+p  q#r"      //0,1.1
        "s+v``w+v```w+vw+v```w+v``w+r"      //0,1
        "s++++++++++++++++++++++++++r"      //0,0.9
        "s+faag+fg+faaaaaag+fg+faag+r"      //0,0.8
        "s+v``w+pq+v``ih``w+pq+v``w+r"      //0,0.7
        "s++++++pq++++pq++++pq++++++r"      //0,0.6
        "zccccg+pxaag pq faayq+fcccc{"      //0,0.5
        "     s+ph``w vw v``iq+r     "      //0,0.4
        "     s+pq          pq+r     "      //0,0.3
        "     s+pq dcc__cce pq+r     "      //0,0.2
        "     s+vw r      s vw+r     "      //0,0.1
        "     s+   r      s   +r     "      //0,0
        "     s+fg r      s fg+r     "      //0,-0.1
        "     s+pq tbbbbbbu pq+r     "      //0,-0.2
        "     s+pq          pq+r     "      //0,-0.3
        "     s+pq faaaaaag pq+r     "      //0,-0.4
        "jbbbbw+vw v``ih``w vw+vbbbbk"      //0,-0.5
        "s++++++++++++pq++++++++++++r"      //0,-0.6
        "s+faag+faaag+pq+faaag+faag+r"      //0,-0.7
        "s+v`iq+v```w+vw+v```w+ph`w+r"      //0,-0.8
        "s#++pq+++++++  +++++++pq++#r"      //0,-0.9
        "|ag+pq+fg+faaaaaag+fg+pq+fa}"      //0,-1
        "l`w+vw+pq+v``ih``w+pq+vw+v`m"      //0,-1.1
        "s++++++pq++++pq++++pq++++++r"      //0,-1.2
        "s+faaaayxaag+pq+faayxaaaag+r"      //0,-1.3
        "s+v````````w+vw+v````````w+r"      //0,-1.4
        "s++++++++++++++++++++++++++r"      //0,-1.5
        "zcccccccccccccccccccccccccc{"      //0,-1.6
        "                            "      //0,-1.7
        "                            ";     //0,-1.8

    // Setup
    bool playFlag = false;
    GameModel gameModel(&mqttClient);
    GameView gameView(&mqttClient);

    // Players
    Player jugador("robot1",{0,-0.85});
    Red red("robot2",{0,0.35});
    Pink pink("robot3", { -0.2,0.35 });
    Cyan cyan("robot4", { -0.15,0.05 });
    Orange orange("robot5", { 0.15 ,0.05 });

    // Configure
    
    gameModel.addRobot(&jugador);
    gameModel.addRobot(&red);
    gameModel.addRobot(&pink);
    gameModel.addRobot(&cyan);
    gameModel.addRobot(&orange);
    gameModel.setGameView(&gameView);

    //game loop
    while (!WindowShouldClose() && mqttClient.isConnected())
    {
        float deltaTime = (float)GetFrameTime();

        // raylib
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("EDAPark Controller", 225, 220, 20, LIGHTGRAY);
        EndDrawing();

        switch (gameModel.gameState)
        {
        case GameStart:

            gameModel.start(maze);
            break;

        case GameStarting:
            if (IsKeyPressed(KEY_SPACE))
            {
                gameModel.gameState = GamePlaying;
            }
            break;

        case GamePlaying:
            gameModel.update(deltaTime);
            break;

        case GameEnding:
            gameView.setMessage(GameViewMessagePlayAgain);

            if (IsKeyPressed(KEY_SPACE))
            {
                gameModel.gameState = GameStart;
            }

            break;

        }

        // Keyboard control
        
        updateInput(&jugador);
        gameView.update(deltaTime);
    }

    CloseWindow();

    cout << "Disconnected." << endl;
}


void updateInput (Player* player)
{
    if (IsKeyDown(KEY_UP))
    {
        player->setDirection(DirectionUp);
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        player->setDirection(DirectionRight);
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        player->setDirection(DirectionDown);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player->setDirection(DirectionLeft);
    }
    else
    {
        player->setDirection(DirectionNone);
    }
}
