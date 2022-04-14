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
    InitWindow(640, 480, "EDAPark Controller");
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
        "     s+vw r      s vw+r     "
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
    GameModel gameModel(&mqttClient);
    GameView gameView(&mqttClient);

    // Players
    Player jugador("robot1",{0,-0.85});

    // Configure
    gameModel.setGameView(&gameView);
    gameModel.addRobot(&jugador);
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

        // Keyboard control
        if (IsKeyDown(KEY_UP))
        {
            // Your code goes here...
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            // Your code goes here...
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            // Your code goes here...
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            // Your code goes here...
        }
        else
        {
            // Your code goes here...
        }

        gameView.update(deltaTime);
    }

    CloseWindow();

    cout << "Disconnected." << endl;
}
