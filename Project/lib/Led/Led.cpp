#include "Led.h"
#include "Arduino.h"

Led::Led(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    isOn = false;
}

void Led::switchOn()
{
    if (!isOn)
    {
        digitalWrite(pin, HIGH);
        isOn = true;
    }
}

void Led::switchOff()
{
    if (isOn)
    {
        digitalWrite(pin, LOW);
        isOn = false;
    }
}