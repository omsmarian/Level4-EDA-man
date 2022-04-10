/*
 * EDA-Man
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Controls an EDA-Man game.
 */

#include <iostream>

#include <raylib.h>

#include "MQTTClient.h"

#include "GameModel.h"
#include "GameView.h"
#include "Player.h"

using namespace std;

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
    InitWindow(640, 480, "EDA-Man Controller");
    SetTargetFPS(60);

    string maze =
        "                            "
        "                            "
        "                            "
        "jbbbbbbbbbbbbonbbbbbbbbbbbbk"
        "s++++++++++++pq++++++++++++r"
        "s+faag+faaag+pq+faaag+faag+r"
        "s#p  q+p   q+pq+p   q+p  q#r"
        "s+v``w+v```w+vw+v```w+v``w+r"
        "s++++++++++++++++++++++++++r"
        "s+faag+fg+faaaaaag+fg+faag+r"
        "s+v``w+pq+v``ih``w+pq+v``w+r"
        "s++++++pq++++pq++++pq++++++r"
        "zccccg+pxaag pq faayq+fcccc{"
        "     s+ph``w vw v``iq+r     "
        "     s+pq          pq+r     "
        "     s+pq dcc__cce pq+r     "
        "jbbbbw+vw r      s vw+vbbbbk"
        "s     +   r      s   +     r"
        "zccccg+fg r      s fg+fcccc{"
        "     s+pq tbbbbbbu pq+r     "
        "     s+pq          pq+r     "
        "     s+pq faaaaaag pq+r     "
        "jbbbbw+vw v``ih``w vw+vbbbbk"
        "s++++++++++++pq++++++++++++r"
        "s+faag+faaag+pq+faaag+faag+r"
        "s+v`iq+v```w+vw+v```w+ph`w+r"
        "s#++pq+++++++  +++++++pq++#r"
        "|ag+pq+fg+faaaaaag+fg+pq+fa}"
        "l`w+vw+pq+v``ih``w+pq+vw+v`m"
        "s++++++pq++++pq++++pq++++++r"
        "s+faaaayxaag+pq+faayxaaaag+r"
        "s+v````````w+vw+v````````w+r"
        "s++++++++++++++++++++++++++r"
        "zcccccccccccccccccccccccccc{"
        "                            "
        "                            ";

    // Setup
    GameModel gameModel(&mqttClient);
    GameView gameView(&mqttClient);
    Player player;

    // Configure
    gameModel.setGameView(&gameView);
    gameModel.addRobot(&player);
    gameModel.start(maze);
    

    while (!WindowShouldClose() && mqttClient.isConnected())
    {
        float deltaTime = (float)GetFrameTime();

        // raylib
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("EDAPark Controller", 225, 220, 20, LIGHTGRAY);
        EndDrawing();

        vector<MQTTMessage> messages = mqttClient.getMessages();

        // Model update
        gameModel.update(deltaTime);

        //MODULO DE PRUEBA DE ANALISIS DE TABLERO Y SETPOINT
/*        if (gameModel.isTileFree({jugador.positionX,jugador.positionZ})
        {
            jugador.setSetpoint();
        }
*/        
        
        if(IsKeyPressed(KEY_UP))
        {
            gameModel.KeyboardInput.upKey = 1;
            gameModel.KeyboardInput.downKey = 0;
            gameModel.KeyboardInput.rightKey = 0;
            gameModel.KeyboardInput.leftKey = 0;
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            gameModel.KeyboardInput.upKey = 0;
            gameModel.KeyboardInput.downKey = 1;
            gameModel.KeyboardInput.rightKey = 0;
            gameModel.KeyboardInput.leftKey = 0;
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            gameModel.KeyboardInput.upKey = 0;
            gameModel.KeyboardInput.downKey = 0;
            gameModel.KeyboardInput.rightKey = 0;
            gameModel.KeyboardInput.leftKey = 1;
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            gameModel.KeyboardInput.upKey = 0;
            gameModel.KeyboardInput.downKey = 0;
            gameModel.KeyboardInput.rightKey = 1;
            gameModel.KeyboardInput.leftKey = 0;
        }
            
            if (gameModel.KeyboardInput.upKey)
            {
                player.movement(0,-1);
            }
            else if (gameModel.KeyboardInput.rightKey)
            {
                player.movement(1, 0);
            }
            else if (gameModel.KeyboardInput.downKey)
            {
                player.movement(0, 1);
            }
            else if (gameModel.KeyboardInput.leftKey)
            {
                player.movement(-1, 0);
            }
        
            /*bool UpKey = false;
            bool DownKey = false;
            bool RightKey = false;
            bool LeftKey = false;*/
        // Keyboard control
        // if (IsKeyPressed(KEY_UP))
        // {
        //     int i = 36;
        //     while(i--)
        //         player.movement(0,-1);
        // }
        // else if (IsKeyPressed(KEY_RIGHT))
        // {
        //     int i = 28;
        //     while (i--)
        //         player.movement(1, 0);
        // }
        // else if (IsKeyPressed(KEY_DOWN))
        // {
        //     int i = 36;
        //     while (i--)
        //         player.movement(0, 1);
        // }
        // else if (IsKeyPressed(KEY_LEFT))
        // {
        //     int i = 28;
        //     while (i--)
        //         player.movement(-1, 0);
        // }
        // else
        // {
        //     // Your code goes here...
        // }

        if(IsKeyPressed(KEY_ZERO))
        {
            player.reset();
        }
            

        gameView.update(deltaTime);
    }

    CloseWindow();

    cout << "Disconnected." << endl;
}
