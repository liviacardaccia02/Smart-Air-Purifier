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
    MQ7->setRegressionMethod(1);
    MQ7->init();
}

void SensorHandler::calibrate()
{
    Serial.print("Calibrating MQ-135 sensor please wait ...");
  float calcR0 = 0;
  for (int i = 1; i <= R0_PRECISION; i++)
  {
    MQ135->update(); // Update data, read the voltage from the analog pin
    calcR0 += MQ135->calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135->setR0(calcR0 / R0_PRECISION);
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

//   Serial.print("Calibrating MQ-7 sensor please wait ...");
//   float calcR0MQ7 = 0;
//   for (int i = 1; i <= R0_PRECISION; i++)
//   {
//     MQ7->update(); // Update data, read the voltage from the analog pin
//     calcR0MQ7 += MQ7->calibrate(RatioMQ135CleanAir);
//     Serial.print(".");
//   }
//   MQ7->setR0(calcR0MQ7 / R0_PRECISION);
//   Serial.println("  done!");
//   Serial.println(calcR0MQ7);

//   if (isinf(calcR0MQ7))
//   {
//     Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
//     while (1)
//       ;
//   }
//   if (calcR0MQ7 == 0)
//   {
//     Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
//     while (1)
//       ;
//   }
}
