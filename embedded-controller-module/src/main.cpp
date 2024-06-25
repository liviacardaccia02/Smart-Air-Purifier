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

const char *ssid = "iPhone di Livia";
const char *password = "kitty123";
const char *mqttBroker = "broker.hivemq.com";
const char *modeTopic = "Smart-air-purifier/control/mode";
const char *mqttUsername = "liviacardaccia";
const char *mqttPassword = "public";
const int mqttPort = 1883;

WiFiClient espClient;
MQTTConnection *mqtt;
WIFIConnection *wifi;
SensorHandler *sensors;
FanController *fan;
Led *redLed;
Led *greenLed;
DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ135("ESP32", VoltageResolution, ADCBitResolution, MQ135PIN, "MQ-135");
MQUnifiedsensor MQ7("ESP32", VoltageResolution, ADCBitResolution, MQ7PIN, "MQ-7");
PurifierStateMachine stateMachine(fan, mqtt);

void setup(void)
{
  delay(500);

  Serial.begin(115200);

  pinMode(PWMMQ7PIN, OUTPUT);
  pinMode(PWMFANPIN, OUTPUT);
  pinMode(FANPIN, INPUT);

  redLed = new Led(REDLEDPIN);
  greenLed = new Led(GREENLEDPIN);
  fan = new FanController();
  mqtt = new MQTTConnection(mqttBroker, mqttPort, mqttUsername, mqttPassword, &espClient, redLed, greenLed);
  wifi = new WIFIConnection(ssid, password, redLed, greenLed);
  sensors = new SensorHandler(&dht, &MQ135, &MQ7);

  delay(100);

  wifi->connect();
  mqtt->connect();
  sensors->setUp();
  sensors->calibrate();
  mqtt->subscribe(modeTopic, [&](char *topic, uint8_t *payload, unsigned int length)
                  {
        String message = String((char*)payload).substring(0, length);
        if (message == "AUTOMATIC") {
            stateMachine.setMode(AUTOMATIC);
        } else if (message == "MANUAL") {
            stateMachine.setMode(MANUAL);
        } else if (message == "ALARM") {
            stateMachine.setMode(ALARM);
        } });

  delay(100);
}

void loop(void)
{
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

  float temperature = doc["temperature"];
  float humidity = doc["humidity"];
  float mq7Value = doc["COlevel"];
  float mq135Value = doc["CO2level"];

  stateMachine.update(temperature, mq7Value, mq135Value, json);
}
