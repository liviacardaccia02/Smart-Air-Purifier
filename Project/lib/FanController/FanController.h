#ifndef __FAN_CONTROLLER__
#define __FAN_CONTROLLER__

#include <Arduino.h>

class FanController
{
private:
    int minTemp;
    int maxTemp;
    int minSpeed;
    int maxSpeed;
public:
    FanController(int minTemp, int maxTemp, int minSpeed, int maxSpeed);
    int calculateFanSpeed(float temp);
};

#endif