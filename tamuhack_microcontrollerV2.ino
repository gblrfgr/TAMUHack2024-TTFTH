#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

// network credentials
const char *ssid = "TAMU_IoT";
const char *serverName = "http://plantsaver.peppersalt.info:5000/moisture";

String server = "";

unsigned long long lastUpdate;

float sense_Pin = 0; // Soil Sensor input at Analog PIN A0
float value = 0;
float reading = 0;
float gain = 0.1;
int count = 0;

const int button = 16;
int temp = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(button, INPUT);

  WiFi.begin(ssid);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi");

  lastUpdate = millis();
}

void loop()
{
  temp = digitalRead(button);
  // looking to see if its divisible by 30 minutes
  value = analogRead(sense_Pin);
  value = (value - 5.0) / (870.0 - 5.0);
  reading = value * gain + reading * (1.0 - gain);
  if (millis() - lastUpdate >= 1800 || temp == HIGH)
  {
    Serial.print("\nMOISTURE LEVEL : ");

    // creates the data to send to website
    String postData = "{\"moisture_level\": " + String(reading) + "}";

    HTTPClient http;
    WiFiClient wifiClient;
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(postData);

    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.printf("[HTTP] Get... code %d\n", httpCode);
      }

      http.end();
    }
    lastUpdate = millis();
  }
}
