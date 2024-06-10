#include "MQTTConnection.h"

MQTTConnection::MQTTConnection(const char *mqttBroker, int mqttPort, const char *mqttUsername,
                               const char *mqttPassword, WiFiClient *espClient, Led *redLed, Led *greenLed)
{
    this->mqttBroker = mqttBroker;
    this->mqttPort = mqttPort;
    this->mqttUsername = mqttUsername;
    this->mqttPassword = mqttPassword;
    this->redLed = redLed;
    this->greenLed = greenLed;

    client.setServer(mqttBroker, mqttPort);
    client.setClient(*espClient);
}

void MQTTConnection::connect()
{
    delay(100);

    if (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        String clientID = "ESP32Client-";
        clientID += String(random(0xffff), HEX);

        while (!client.connect(clientID.c_str(), mqttUsername, mqttPassword))
        {
            delay(500);
            Serial.print(".");
        }

        greenLed->switchOn();
        redLed->switchOff();
        Serial.println("Connected to MQTT");
    }
}

void MQTTConnection::publish(const char *topic, const char *message)
{
    client.publish(topic, message);
}

void MQTTConnection::subscribe(const char *firstTopic, const char *secondTopic, std::function<void(char *, uint8_t *, unsigned int)> callback)
{
    client.setCallback(callback);
    client.subscribe(firstTopic);
    client.subscribe(secondTopic);
}

void MQTTConnection::checkConnection()
{
    if (!client.connected())
    {
        redLed->switchOn();
        greenLed->switchOff();
        this->connect();
    }
}

void MQTTConnection::update()
{
    client.loop();
}
