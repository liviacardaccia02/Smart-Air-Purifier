#ifndef __MQTT_MANAGER__
#define __MQTT_MANAGER__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Led.h>

class MQTTManager
{
public:
    MQTTManager(const char *mqttBroker, int mqttPort, const char *mqttUsername,
                const char *mqttPassword, WiFiClient *espClient, Led *redLed, Led *greenLed);
    void connect();
    void publish(const char *topic, const char *message);
    void subscribe(const char *topic, std::function<void(char*, uint8_t*, unsigned int)> callback);
    void checkConnection();
    void update();

private:
    WiFiClient espClient;
    PubSubClient client;
    Led *redLed;
    Led *greenLed;
    const char *mqttBroker;
    const char *mqttUsername;
    const char *mqttPassword;
    int mqttPort;
};

#endif