#include "FanController.h"

FanController::FanController(int minTemp, int maxTemp, int minSpeed, int maxSpeed)
{
    this->minTemp = minTemp;
    this->maxTemp = maxTemp;
    this->minSpeed = minSpeed;
    this->maxSpeed = maxSpeed;
}

int FanController::calculateFanSpeed(float temp)
{
    int temperature = constrain(temp, minTemp, maxTemp);
    return map(temperature, minTemp, maxTemp, minSpeed, maxSpeed);
}