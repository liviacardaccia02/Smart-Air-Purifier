#include "SensorHandler.h"

SensorHandler::SensorHandler(DHT *dht, MQUnifiedsensor *MQ135, MQUnifiedsensor *MQ7, ...)
{
  this->dht = dht;
  this->MQ135 = MQ135;
  this->MQ7 = MQ7;
}

void SensorHandler::setUp()
{
  dht->begin();
  MQ135->setRegressionMethod(1);
  MQ135->init();
  // MQ7->setRegressionMethod(1);
  // MQ7->setA(99.042);
  // MQ7->setB(-1.518);
  // MQ7->init();
}

void SensorHandler::calibrate()
{
  Serial.print("Calibrating MQ-135 sensor please wait ...");
  float calcR0 = 0;
  for (int i = 1; i <= R0PRECISION; i++)
  {
    MQ135->update(); // Update data, read the voltage from the analog pin
    calcR0 += MQ135->calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135->setR0(calcR0 / R0PRECISION);
  Serial.println("  done!");
  Serial.println(calcR0);

  if (isinf(calcR0))
  {
    Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1)
      ;
  }

  // Serial.print("Calibrating MQ7 sensor please wait ...");
  // calcR0 = 0;
  // for (int i = 1; i <= 10; i++)
  // {
  //   MQ7->update(); // Update data, the arduino will read the voltage from the analog pin
  //   calcR0 += MQ7->calibrate(RatioMQ7CleanAir);
  //   Serial.print(".");
  // }
  // MQ7->setR0(calcR0 / 10);
  // Serial.println("  done!.");

  // if (isinf(calcR0))
  // {
  //   Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
  //   while (1)
  //     ;
  // }
  // if (calcR0 == 0)
  // {
  //   Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
  //   while (1)
  //     ;
  // }
}

void SensorHandler::read()
{
  // MQ7->update();
  // MQ7->setA(99.042);
  // MQ7->setB(-1.518);
  // MQ7CO = MQ7->readSensor();
  MQ7CO = 0; // TODO: togliere

  MQ135->update();
  MQ135->setA(605.18);
  MQ135->setB(-3.937);
  MQ135CO = MQ135->readSensor();

  MQ135->setA(110.47);
  MQ135->setB(-2.862);
  CO2 = MQ135->readSensor();

  temperature = dht->readTemperature();
  humidity = dht->readHumidity();

  delay(500);
}

void SensorHandler::debug()
{
  Serial.println("*** Values from sensors ***");
  Serial.println("|   MQ7CO   |   MQ135CO   |  CO2  |  Temperature  |  Humidity  |");
  Serial.print("|   ");
  Serial.print(MQ7CO);
  Serial.print("|   ");
  Serial.print(MQ135CO);
  Serial.print("   |   ");
  Serial.print(CO2 + PollutionOffeset);
  Serial.print("   |   ");
  Serial.print(temperature);
  Serial.print("   |   ");
  Serial.print(humidity);
  Serial.println("   |");
}

String SensorHandler::encode()
{
  JsonDocument jsonDoc;

  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["COlevel"] = MQ7CO;
  jsonDoc["CO2level"] = CO2 + PollutionOffeset;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  return jsonString;
}
