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
}

void FanController::initializeSpeedTable()
{
    speedTable.insert(make_pair(0.0f, minSpeed));
    speedTable.insert(make_pair(0.5f, (minSpeed + maxSpeed) / 2));
    speedTable.insert(make_pair(1.0f, maxSpeed));
}

int FanController::calculateFanSpeed(float temp, float coLevel, float co2Level)
{
    float weightedScore = temp * tWeight + co2Level * co2Weight + coLevel * coWeight;
    auto bound = speedTable.lower_bound(weightedScore);

    if (bound == speedTable.end()) {
      return speedTable.rbegin()->second;
    }
    return bound->second;
}