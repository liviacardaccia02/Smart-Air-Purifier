#include "PurifierStateMachine.h"

PurifierStateMachine::PurifierStateMachine(FanController *fanController, MQTTConnection *mqtt)
{
    this->fanController = fanController;
    this->mqtt = mqtt;
    this->currentMode = AUTOMATIC;
}

void PurifierStateMachine::setMode(OperatingMode mode)
{
    this->currentMode = mode;
}

void PurifierStateMachine::update(float temp, float coLevel, float co2Level, String json)
{
    if (fanController->isAnomalyDetected(temp, coLevel, co2Level))
    {
        setMode(ALARM);
    }

    switch (currentMode)
    {
    case AUTOMATIC:
        handleAutomaticMode(temp, coLevel, co2Level, json);
        break;
    case MANUAL:
        handleManualMode(json);
        break;
    case ALARM:
        handleAlarmMode(temp, coLevel, co2Level, json);
        break;
    }
}

void PurifierStateMachine::handleAutomaticMode(float temp, float coLevel, float co2Level, String json)
{
    int speed = fanController->calculateFanSpeed(temp, coLevel, co2Level);
    fanController->setSpeed(speed);
    String speedString = fanController->encodeSpeed(speed);
    speedString = "{\"speed\":\"" + speedString + "\"}";
    mqtt->publish("Smart-air-purifier/fan/speed", speedString.c_str());
    mqtt->publish("Smart-air-purifier/sensor/data", json.c_str());
}

void PurifierStateMachine::handleManualMode(String json)
{
    int speed = 0;
    mqtt->subscribe("Smart-air-purifier/fan/speed", [&](char *topic, uint8_t *payload, unsigned int length)
                    {
        String message = String((char *)payload).substring(0, length);
        speed = fanController->decodeSpeed(message); });
    fanController->setSpeed(speed);
    mqtt->publish("Smart-air-purifier/sensor/data", json.c_str());
}

void PurifierStateMachine::handleAlarmMode(float temp, float coLevel, float co2Level, String json)
{
    this->handleAutomaticMode(temp, coLevel, co2Level, json);
    mqtt->publish("Smart-air-purifier/alarm", "true");

    if (!fanController->isAnomalyDetected(temp, coLevel, co2Level))
    {
        setMode(AUTOMATIC);
    }
}
