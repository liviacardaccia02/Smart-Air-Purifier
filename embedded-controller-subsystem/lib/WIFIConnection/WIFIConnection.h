#ifndef __WIFI_CONNECTION__
#define __WIFI_CONNECTION__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Led.h>
#include <ConnectionHandler.h>

class WIFIConnection : public ConnectionHandler
{
public:
    WIFIConnection(const char *ssid, const char *password, Led *redLed, Led *greenLed);
    void connect() override;
    void checkConnection() override;

private:
    Led *redLed;
    Led *greenLed;
    const char *ssid;
    const char *password;
};

#endif