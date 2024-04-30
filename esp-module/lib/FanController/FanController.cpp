#include "FanController.h"

FanController::FanController()
{
    this->minSpeed = MIN_SPEED;
    this->maxSpeed = MAX_SPEED;
    this->minTemp = MIN_TEMP;
    this->maxTemp = MAX_TEMP;
    this->minCoLevel = MIN_CO_LEVEL;
    this->maxCoLevel = MAX_CO_LEVEL;
    this->minCo2Level = MIN_CO2_LEVEL;
    this->maxCo2Level = MAX_CO2_LEVEL;
}

// int FanController::calculateFanSpeed(float temp)
// {
//     int temperature = constrain(temp, minTemp, maxTemp);
//     return map(temperature, minTemp, maxTemp, minSpeed, maxSpeed);
// }

int FanController::calculateFanSpeed(float temp, float coLevel, float co2Level)
{
    int temperature = constrain(temp, minTemp, maxTemp);
    int coSpeed = map(coLevel, minCoLevel, maxCoLevel, minSpeed, maxSpeed);
    int co2Speed = map(co2Level, minCo2Level, maxCo2Level, minSpeed, maxSpeed);

    // Calculate the average speed based on temperature, CO level, and CO2 level
    int averageSpeed = (temperature + coSpeed + co2Speed) / 3;

    return constrain(averageSpeed, minSpeed, maxSpeed);
}