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
const char *topic = "Smart-air-purifier";
const char *speedTopic = "Smart-air-purifier/speed";
const char *mqttUsername = "liviacardaccia";
const char *mqttPassword = "public";
const int mqttPort = 1883;
unsigned long int lastHeatingTime = 0;
bool highVoltage = true;
int fanSpeed = 0;

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

// void heatingTask(void *parameter);

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
    fanSpeed = atoi(str);
  }
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
  mqttManager->subscribe(speedTopic, callback);

  delay(100);
  // xTaskCreate(heatingTask, "HeatingTask", 10000, NULL, 1, NULL);
}

void loop(void)
{
  mqttManager->update();
  wifiManager->checkConnection();
  mqttManager->checkConnection();

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

  // Retrieve the numerical values from the JSON
  float temperature = doc["temperature"];
  float humidity = doc["humidity"];
  float mq7Value = doc["mq7Value"];
  float mq135Value = doc["mq135Value"];

  // Calculate the fan speed using the numerical values
  fanSpeed = fanController->calculateFanSpeed(temperature, mq7Value, mq135Value);
  analogWrite(PWMFANPIN, fanSpeed);
  Serial.println(fanSpeed);

  sensorHandler->read();
  //sensorHandler->debug();

  mqttManager->publish(topic, sensorHandler->encode().c_str());
}

// void heatingTask(void *parameter)
// {
//   // Code for debug purposes, to be removed
//   uint32_t stackPointer = uxTaskGetStackHighWaterMark(NULL);
//   Serial.print("Stack Pointer Value: ");
//   Serial.println(stackPointer);

//   if (highVoltage)
//   {
//     lastHeatingTime = millis();
//     while (millis() - lastHeatingTime <= (60 * 1000))
//     {
//       // VH 5 Volts
//       analogWrite(5, 255); // 255 is DC 5V output
//       MQ7.update();        // Update data, the arduino will read the voltage from the analog pin
//       MQ7.readSensor();    // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
//       MQ7.serialDebug();   // Will print the table on the serial port
//       delay(500);          // Sampling frequency
//     }
//     highVoltage = !highVoltage;
//   }
//   else
//   {
//     lastHeatingTime = millis();
//     while (millis() - lastHeatingTime <= (90 * 1000))
//     {
//       // VH 1.4 Volts
//       analogWrite(5, 20); // 255 is 100%, 20.4 is aprox 8% of Duty cycle for 90s
//       MQ7.update();       // Update data, the arduino will read the voltage from the analog pin
//       MQ7.readSensor();   // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
//       MQ7.serialDebug();  // Will print the table on the serial port
//       delay(500);         // Sampling frequency
//     }
//     highVoltage = !highVoltage;
//   }
// }