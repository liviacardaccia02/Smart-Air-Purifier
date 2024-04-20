#ifndef __SENSOR_HANDLER__
#define __SENSOR_HANDLER__

#include <MQUnifiedsensor.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>

#define R0_PRECISION 100
#define RatioMQ135CleanAir 3.6  //RS / R0 = 3.6 ppm of clean air

class SensorHandler
{
private:
    DHT *dht;
    MQUnifiedsensor *MQ135;
    MQUnifiedsensor *MQ7;

public:
    SensorHandler(DHT *dht, MQUnifiedsensor *MQ135, MQUnifiedsensor *MQ7, ...);
    void setUp();
    void calibrate();
};

#endif