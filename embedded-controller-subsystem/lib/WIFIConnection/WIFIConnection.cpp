#include "WIFIConnection.h"

WIFIConnection::WIFIConnection(const char *ssid, const char *password, Led *redLed, Led *greenLed)
{
    this->ssid = ssid;
    this->password = password;
    this->redLed = redLed;
    this->greenLed = greenLed;
}

void WIFIConnection::connect()
{
    delay(100);

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi...");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        greenLed->switchOn();
        redLed->switchOff();
        Serial.println("Connected to WiFi");
        Serial.println("IP address: ");
        Serial.print(WiFi.localIP());
    }
}

void WIFIConnection::checkConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        redLed->switchOn();
        greenLed->switchOff();
        this->connect();
    }
}
