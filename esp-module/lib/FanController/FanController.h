#ifndef __FAN_CONTROLLER__
#define __FAN_CONTROLLER__

#define MIN_SPEED 0
#define MAX_SPEED 255
#define MIN_TEMP 0
#define MAX_TEMP 50
#define MIN_CO_LEVEL 0.1 // TODO controllare
#define MAX_CO_LEVEL 10000
#define MIN_CO2_LEVEL 200
#define MAX_CO2_LEVEL 10000
#define TEMPERATURE_WEIGHT 0.4f
#define CO2_LEVEL_WEIGHT 0.3f
#define CO_LEVEL_WEIGHT 0.3f

#include <Arduino.h>
#include <iostream>
#include <map>

using namespace std;

class FanController
{
private:
    int minSpeed;
    int maxSpeed;
    int minTemp;
    int maxTemp;
    int minCoLevel;
    int maxCoLevel;
    int minCo2Level;
    int maxCo2Level;
    float tWeight;
    float co2Weight;
    float coWeight;
    std::map<float, int> speedTable;

    void initializeSpeedTable();

public:
    FanController();
    int calculateFanSpeed(float temp, float coLevel, float co2Level);
};

#endif