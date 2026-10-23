#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <MQUnifiedsensor.h>
#include <MQTTConnection.h>
#include <WIFIConnection.h>
#include <SensorHandler.h>
#include <FanController.h>
#include <PurifierStateMachine.h>
#include <Led.h>
#include <utils.h>
#include <OperatingMode.h>

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif
#ifndef MQTT_USERNAME
#define MQTT_USERNAME ""
#endif
#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD ""
#endif

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqttBroker = "broker.hivemq.com";
const char *modeTopic = "Smart-air-purifier/control/mode";
const char *fanCommandTopic = "Smart-air-purifier/control/fan-speed";
const char *mqttUsername = MQTT_USERNAME;
const char *mqttPassword = MQTT_PASSWORD;
const int mqttPort = 1883;

WiFiClient espClient;
MQTTConnection *mqtt = nullptr;
WIFIConnection *wifi = nullptr;
SensorHandler *sensors = nullptr;
FanController *fan = nullptr;
PurifierStateMachine *stateMachine = nullptr;
Led *redLed = nullptr;
Led *greenLed = nullptr;
DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ135("ESP32", VoltageResolution, ADCBitResolution, MQ135PIN, "MQ-135");
MQUnifiedsensor MQ7("ESP32", VoltageResolution, ADCBitResolution, MQ7PIN, "MQ-7");

void handleControlMessage(char *topic, uint8_t *payload, unsigned int length)
{
  String message;
  message.reserve(length);
  for (unsigned int i = 0; i < length; ++i)
    message += static_cast<char>(payload[i]);

  if (String(topic) == modeTopic && stateMachine != nullptr)
  {
    if (message == "AUTOMATIC") stateMachine->setMode(AUTOMATIC);
    else if (message == "MANUAL") stateMachine->setMode(MANUAL);
    else if (message == "ALARM") stateMachine->setMode(ALARM);
  }
  else if (String(topic) == fanCommandTopic && fan != nullptr && stateMachine != nullptr)
  {
    stateMachine->setManualFanSpeed(fan->decodeSpeed(message));
  }
}

void setup(void)
{
  delay(500);
  Serial.begin(115200);

  if (ssid[0] == '\0' || password[0] == '\0')
  {
    Serial.println("Startup stopped: configure WIFI_SSID and WIFI_PASSWORD in local PlatformIO build flags.");
    while (true) delay(1000);
  }

  pinMode(PWMMQ7PIN, OUTPUT);
  pinMode(PWMFANPIN, OUTPUT);
  pinMode(FANPIN, INPUT);

  redLed = new Led(REDLEDPIN);
  greenLed = new Led(GREENLEDPIN);
  fan = new FanController();
  mqtt = new MQTTConnection(mqttBroker, mqttPort, mqttUsername, mqttPassword, &espClient, redLed, greenLed);
  wifi = new WIFIConnection(ssid, password, redLed, greenLed);
  sensors = new SensorHandler(&dht, &MQ135, &MQ7);
  stateMachine = new PurifierStateMachine(fan, mqtt);

  wifi->connect();
  mqtt->connect();
  sensors->setUp();
  sensors->calibrate();
  mqtt->subscribe(modeTopic, handleControlMessage);
  mqtt->subscribe(fanCommandTopic, handleControlMessage);
}

void loop(void)
{
  if (mqtt == nullptr || wifi == nullptr || sensors == nullptr || stateMachine == nullptr)
    return;

  mqtt->update();
  wifi->checkConnection();
  mqtt->checkConnection();
  sensors->read();

  String json = sensors->encode();
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  float temperature = doc["temperature"] | 0.0f;
  float mq7Value = doc["COlevel"] | 0.0f;
  float mq135Value = doc["CO2level"] | 0.0f;
  stateMachine->update(temperature, mq7Value, mq135Value, json);
}
