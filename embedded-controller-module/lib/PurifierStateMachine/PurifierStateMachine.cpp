#include "PurifierStateMachine.h"

PurifierStateMachine::PurifierStateMachine(FanController *fanController, MQTTConnection *mqtt)
{
    this->fanController = fanController;
    this->mqtt = mqtt;
    this->currentMode = AUTOMATIC;
    this->manualFanSpeed = 127;
}

void PurifierStateMachine::setMode(OperatingMode mode)
{
    this->currentMode = mode;
}

void PurifierStateMachine::setManualFanSpeed(int speed)
{
    if (speed == 0 || speed == 127 || speed == 255)
    {
        manualFanSpeed = speed;
    }
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
    String speedString = "{\"speed\":\"" + fanController->encodeSpeed(speed) + "\"}";
    mqtt->publish("Smart-air-purifier/fan/speed", speedString.c_str());
    mqtt->publish("Smart-air-purifier/sensor/data", json.c_str());
}

void PurifierStateMachine::handleManualMode(String json)
{
    fanController->setSpeed(manualFanSpeed);
    mqtt->publish("Smart-air-purifier/fan/speed", ("{\"speed\":\"" + fanController->encodeSpeed(manualFanSpeed) + "\"}").c_str());
    mqtt->publish("Smart-air-purifier/sensor/data", json.c_str());
}

void PurifierStateMachine::handleAlarmMode(float temp, float coLevel, float co2Level, String json)
{
    handleAutomaticMode(temp, coLevel, co2Level, json);
    mqtt->publish("Smart-air-purifier/alarm", "true");

    if (!fanController->isAnomalyDetected(temp, coLevel, co2Level))
    {
        setMode(AUTOMATIC);
        mqtt->publish("Smart-air-purifier/alarm", "false");
    }
}
