#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <MQUnifiedsensor.h>
#include <utils.h>
#include <MQTTManager.h>
#include <WiFiManager.h>
#include <SensorHandler.h>
#include <FanController.h>
#include <Led.h>

const char *ssid = "iPhone di Livia";
const char *password = "kitty123";
const char *mqttBroker = "broker.hivemq.com";
const char *COtopic = "PoliCO";
const char *tempTopic = "PoliTemp";
const char *mqttUsername = "liviacardaccia";
const char *mqttPassword = "public";
const int mqttPort = 1883;
const int minTemp = 26;
const int maxTemp = 28;
const int minSpeed = 0;
const int maxSpeed = 255;

WiFiClient espClient;

MQTTManager *mqttManager;
WiFiManager *wifiManager;
SensorHandler *sensorHandler;
FanController *fanController;
Led *redLed;
Led *greenLed;

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ135("ESP32", VoltageResolution, ADCBitResolution, MQ135PIN, "MQ-135");
MQUnifiedsensor MQ7("ESP32", VoltageResolution, ADCBitResolution, MQ7PIN, "MQ-7");

float readDHTTemperature();
float readDHTHumidity();

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
}

void handleRoot()
{
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>ESP32 DHT Server!</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity());
  server.send(200, "text/html", msg);
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
  fanController = new FanController(minTemp, maxTemp, minSpeed, maxSpeed);
  mqttManager = new MQTTManager(mqttBroker, mqttPort, mqttUsername, mqttPassword, &espClient, redLed, greenLed);
  wifiManager = new WiFiManager(ssid, password, redLed, greenLed);
  sensorHandler = new SensorHandler(&dht, &MQ135, &MQ7);

  delay(100);

  wifiManager->connect();
  mqttManager->connect();
  sensorHandler->setUp();
  // mqttManager->subscribe(topic, callback);

  delay(100);

  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");

  // sensorHandler->calibrate();
}

void loop(void)
{
  server.handleClient();

  mqttManager->update();
  wifiManager->checkConnection();
  mqttManager->checkConnection();

  int speed = fanController->calculateFanSpeed(dht.readTemperature());
  analogWrite(PWMFANPIN, speed);

  sensorHandler->heatMQ7(PWMMQ7PIN);
  sensorHandler->read();
  sensorHandler->debug();
}

float readDHTTemperature()
{
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else
  {
    // Serial.println(t);
    String strValue = "Temperature " + String(t) + "°C";
    mqttManager->publish(tempTopic, strValue.c_str());
    return t;
  }
}

float readDHTHumidity()
{
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else
  {
    // Serial.println(h);
    String strValue = "Humidity " + String(h) + "%";
    mqttManager->publish(tempTopic, strValue.c_str());
    return h;
  }
}
