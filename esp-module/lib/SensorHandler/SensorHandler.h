#ifndef __SENSOR_HANDLER__
#define __SENSOR_HANDLER__

#include <MQUnifiedsensor.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT_U.h>
#include <DHT.h>

#define R0PRECISION 100
#define RatioMQ135CleanAir 3.6  //RS / R0 = 3.6 ppm of clean air
#define RatioMQ7CleanAir 27.5
#define PollutionOffeset 421

class SensorHandler
{
private:
    DHT *dht;
    MQUnifiedsensor *MQ135;
    MQUnifiedsensor *MQ7;
    float MQ7CO;
    float MQ135CO;
    float alcohol;
    float CO2;
    float toluen;
    float NH4;
    float aceton;
    float temperature;
    float humidity;

public:
    SensorHandler(DHT *dht, MQUnifiedsensor *MQ135, MQUnifiedsensor *MQ7, ...);
    void setUp();
    void calibrate();
    void read();
    void debug();
    String encode();
};

#endif