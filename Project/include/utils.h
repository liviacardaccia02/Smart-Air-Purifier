#ifndef __UTILS__
#define __UTILS__

#define BOARDTYPE "ESP32"
#define DHTTYPE DHT11
#define FANPIN 1
#define PWMPIN 2
#define DHTPIN 4 
#define MQ135PIN 5
#define MQ7PIN 6
#define GREENLEDPIN 43
#define REDLEDPIN 44
#define R0_PRECISION 100 // TODO: to be removed (its already in SensorHandler.h)
#define RatioMQ135CleanAir 3.6  //RS / R0 = 3.6 ppm of clean air

#endif