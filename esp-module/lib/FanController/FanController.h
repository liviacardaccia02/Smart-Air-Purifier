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

#include <Arduino.h>

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
public:
    FanController();
    int calculateFanSpeed(float temp, float coLevel, float co2Level);
};

#endif