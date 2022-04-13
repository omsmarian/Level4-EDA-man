#include "Player.h"

#define MODULO(x) ((x>0)?(x):(-x))

Player::Player (Vector2 origenCoords, std::string robotLabel, int imagePointer)
{
    this->posXY = origenCoords;
    this->robotDirection = DirectionNone;
    this->imageIndex = imagePointer;
    this->robotId = robotLabel;
}

int Player::IsRobotInPlace ()
{
    //tomo las coordenadas reales del robot fisico
    Vector3 realCoordenates .......... ; //completar con los mensajes recibidos

    //tomo las coordenadas virtuales del robot en XY
    Setpoint virtualCoordenates = this->getSetpoint(this->posXY);

    //tomo el modulo de las distancias en X e Y entre las coordenadas reales y virtuales
    float Xdifference = virtualCoordenates.position.x - realCoordenates.x;
    Xdifference = MODULO(Xdifference);

    float Ydifference = virtualCoordenates.position.y - realCoordenates.z;
    Ydifference = MODULO(Ydifference);

    if(Xdifference > 0.05F || Ydifference > 0.05F) //0.05 = 5 cm
        return 0; //aun no llega a su posicion

    return 1; //caso en el que el robot esta donde deberia
}

void Player::reset()
{
    this->liftTo(this->spawn);
}
