#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <MQUnifiedsensor.h>
#include <MQTTManager.h>
#include <WiFiManager.h>
#include <SensorHandler.h>
#include <FanController.h>
#include <Led.h>
#include <utils.h>

const char *ssid = "iPhone di Livia";
const char *password = "kitty123";
const char *mqttBroker = "broker.hivemq.com";
const char *mainTopic = "Smart-air-purifier";
const char *speedTopic = "Smart-air-purifier/speed";
const char *modeTopic = "Smart-air-purifier/mode";
const char *mqttUsername = "liviacardaccia";
const char *mqttPassword = "public";
const int mqttPort = 1883;
bool messageReceived = false;
int speed = 0;
string operatingMode = "AUTOMATIC";

WiFiClient espClient;
MQTTManager *mqttManager;
WiFiManager *wifiManager;
SensorHandler *sensorHandler;
FanController *fanController;
Led *redLed;
Led *greenLed;
DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ135("ESP32", VoltageResolution, ADCBitResolution, MQ135PIN, "MQ-135");
MQUnifiedsensor MQ7("ESP32", VoltageResolution, ADCBitResolution, MQ7PIN, "MQ-7");

void callback(char *topic, u_int8_t *payload, unsigned int length)
{
  Serial.print("Message arrived on topic [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, speedTopic) == 0)
  {
    char str[(sizeof(payload)) + 1];
    memcpy(str, payload, sizeof(payload));
    str[sizeof(payload)] = 0;
    speed = atoi(str);
  }

  if (strcmp(topic, modeTopic) == 0)
  {
    char str[(sizeof(payload)) + 1];
    memcpy(str, payload, sizeof(payload));
    str[sizeof(payload)] = 0;
    operatingMode = atoi(str);
  }

  messageReceived = true;
}

void setup(void)
{
  delay(500);

  Serial.begin(115200);

  pinMode(PWMMQ7PIN, OUTPUT);
  pinMode(PWMFANPIN, OUTPUT);
  pinMode(FANPIN, INPUT);

  redLed = new Led(REDLEDPIN);
  greenLed = new Led(GREENLEDPIN);
  fanController = new FanController();
  mqttManager = new MQTTManager(mqttBroker, mqttPort, mqttUsername, mqttPassword, &espClient, redLed, greenLed);
  wifiManager = new WiFiManager(ssid, password, redLed, greenLed);
  sensorHandler = new SensorHandler(&dht, &MQ135, &MQ7);

  delay(100);

  wifiManager->connect();
  mqttManager->connect();
  sensorHandler->setUp();
  sensorHandler->calibrate();
  mqttManager->subscribe(speedTopic, modeTopic, callback);

  delay(100);
}

void loop(void)
{
  mqttManager->update();
  wifiManager->checkConnection();
  mqttManager->checkConnection();
  sensorHandler->read();

  String json = sensorHandler->encode();

  // Parse the JSON-like format string
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);

  // Check if parsing was successful
  if (error)
  {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // TODO change logic for mode controlling

  // Retrieve the numerical values from the JSON
  float temperature = doc["temperature"];
  float humidity = doc["humidity"];
  float mq7Value = doc["mq7Value"];
  float mq135Value = doc["mq135Value"];

  // Calculate the fan speed using the numerical values
  speed = fanController->calculateFanSpeed(temperature, mq7Value, mq135Value);
  analogWrite(PWMFANPIN, speed);
  Serial.println(speed); // TODO remove

  mqttManager->publish(mainTopic, json.c_str());
}
