#ifndef __PURIFIER_STATE_MACHINE__
#define __PURIFIER_STATE_MACHINE__

#include <FanController.h>
#include <MQTTConnection.h>
#include <OperatingMode.h>

class PurifierStateMachine
{
public:
    PurifierStateMachine(FanController *fanController, MQTTConnection *mqtt);
    void setMode(OperatingMode mode);
    void update(float temp, float coLevel, float co2Level, String json);

private:
    OperatingMode currentMode;
    FanController *fanController;
    MQTTConnection *mqtt;

    void handleAutomaticMode(float temp, float coLevel, float co2Level, String json);
    void handleManualMode(String json);
    void handleAlarmMode(float temp, float coLevel, float co2Level, String json);
};

#endif
