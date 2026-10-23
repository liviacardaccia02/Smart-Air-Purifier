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
    this->tWeight = TEMPERATURE_WEIGHT;
    this->co2Weight = CO2_LEVEL_WEIGHT;
    this->coWeight = CO_LEVEL_WEIGHT;
    initializeSpeedTable();
}

void FanController::initializeSpeedTable()
{
    speedTable.insert(make_pair(0.0f, minSpeed));
    speedTable.insert(make_pair(0.5f, (minSpeed + maxSpeed) / 2));
    speedTable.insert(make_pair(1.0f, maxSpeed));
}

bool FanController::isAnomalyDetected(float temp, float coLevel, float co2Level)
{
    return temp > maxTemp || coLevel > maxCoLevel || co2Level > maxCo2Level;
}

int FanController::calculateFanSpeed(float temp, float coLevel, float co2Level)
{
    float weightedScore = temp * tWeight + co2Level * co2Weight + coLevel * coWeight;
    auto bound = speedTable.lower_bound(weightedScore);

    if (bound == speedTable.end())
    {
        return speedTable.rbegin()->second;
    }
    return bound->second;
}

void FanController::setSpeed(int speed)
{
    analogWrite(PWMFANPIN, speed);
}

String FanController::encodeSpeed(int speed)
{
    if (speed == 0) return "OFF";
    if (speed > 0 && speed < 127) return "MEDIUM";
    if (speed >= 127) return "HIGH";
    return "OFF";
}

int FanController::decodeSpeed(String speedString)
{
    if (speedString == "OFF") return 0;
    if (speedString == "MEDIUM") return 127;
    if (speedString == "HIGH") return 255;
    return -1;
}
