/**
 * EDA-Man
 *
 * @copyright Copyright (C) 2022
 * @author Marc S. Ressl
 * 
 * @brief Controls an EDA-Man game.
 */

#include <iostream>
#include <windows.h>
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
        "     s+   r      s   +r     "
        "     s+fg r      s fg+r     "
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

    // Configure
    gameModel.setGameView(&gameView);
    Vector2 coordenadasDeOrigen = {0,0};

    Player jugador(coordenadasDeOrigen, "robot1", 1);
    gameModel.addRobot(&jugador);
    /*
    coordenadasDeOrigen = {12,0};
    Fantasma rojo(coordenadasDeOrigen, "robot2", 7);
    gameModel.addRobot(&rojo);

    coordenadasDeOrigen = {12,7};
    Fantasma naranja(coordenadasDeOrigen, "robot3", 9);
    gameModel.addRobot(&naranja);

    coordenadasDeOrigen = {3,-1};
    Fantasma cyan(coordenadasDeOrigen, "robot4", 11);
    gameModel.addRobot(&cyan);

    Fantasma rosa(coordenadasDeOrigen, "robot5", 13);
    gameModel.addRobot(&rosa);
    */
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
        
        if(jugador.IsRobotInPlace())
        {
            // Keyboard control
            if (IsKeyDown(KEY_UP))
            {
                jugador.robotDirection = DirectionUp;
                //jugador.posXY.y++;
                //jugador.movement(jugador.posXY);
            }
            else if (IsKeyDown(KEY_RIGHT))
            {
                jugador.robotDirection = DirectionRight;
                //jugador.posXY.x++;
                //jugador.movement(jugador.posXY);
            }
            else if (IsKeyDown(KEY_DOWN))
            {
                jugador.robotDirection = DirectionDown;
                //jugador.posXY.y--;
                //jugador.movement(jugador.posXY);
            }
            else if (IsKeyDown(KEY_LEFT))
            {
                jugador.robotDirection = DirectionLeft;
                //jugador.posXY.x--;
                //jugador.movement(jugador.posXY);
            }
            else
            {
                // Your code goes here...
            }
        }
        
        //vuelve al origen (facilita las pruebas)
        if(IsKeyDown(KEY_ZERO))
        {
            jugador.reset();
        }

        if(nextPosition.x > 5 ||nextPosition.x <-5 ||nextPosition.y <-5 ||nextPosition.y > 5 )
        {
            nextPosition = {0,0};
            jugador.movement(nextPosition);
        }

        //Sleep(500);

        gameView.update(deltaTime);
    }

    CloseWindow();

    cout << "Disconnected." << endl;
}
