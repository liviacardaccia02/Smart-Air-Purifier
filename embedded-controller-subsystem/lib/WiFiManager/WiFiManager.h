#ifndef __WIFI_MANAGER__
#define __WIFI_MANAGER__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Led.h>

class WiFiManager
{
public:
    WiFiManager(const char *ssid, const char *password, Led *redLed, Led *greenLed);
    void connect();
    void checkConnection();
private:
    Led *redLed;
    Led *greenLed;
    const char *ssid;
    const char *password;
};

#endif