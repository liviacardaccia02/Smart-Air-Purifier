#ifndef __WIFI_MANAGER__
#define __WIFI_MANAGER__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Led.h>

class WiFiManager
{
private:
    Led *redLed;
    Led *greenLed;
    const char *ssid;
    const char *password;
public:
    WiFiManager(const char *ssid, const char *password, Led *redLed, Led *greenLed);
    void connect();
    void checkConnection();
};

#endif