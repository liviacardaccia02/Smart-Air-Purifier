#ifndef __MQTT_CONNECTION__
#define __MQTT_CONNECTION__

#include <WiFi.h>
#include <PubSubClient.h>
#include <Led.h>
#include <ConnectionHandler.h>

class MQTTConnection : public ConnectionHandler
{
public:
    MQTTConnection(const char *mqttBroker, int mqttPort, const char *mqttUsername,
                   const char *mqttPassword, WiFiClient *espClient, Led *redLed, Led *greenLed);
    void connect() override;
    void checkConnection() override;
    void publish(const char *topic, const char *message);
    void subscribe(const char *firstTopic, const char *secondTopic, std::function<void(char *, uint8_t *, unsigned int)> callback);
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