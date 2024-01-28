#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
  #include <NTPClient.h>
  #include <WiFiUdp.h>
#endif

//network credentials
const char* ssid = "TAMU_IoT";
const char* serverName = "http://plantsaver.peppersalt.info:5000/moisture";

String server = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

float sense_Pin= 0; // Soil Sensor input at Analog PIN A0
float value= 0;
int count = 0;

const int button = 16;
int temp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(button, INPUT);

  WiFi.begin(ssid);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi");

  // Initialize a NTPClient to get time
  timeClient.begin();
  
  }

void loop() {
  // gets the clock time
  int time = timeClient.getMinutes();
  temp = digitalRead(button);
  //looking to see if its divisible by 30 minutes
  if (time % 2 == 0){
    if (count == 0){//gets moisture level from plant
      count = count + 1;
      Serial.print("\nMOISTURE LEVEL : ");
      value= analogRead(sense_Pin);
      value= (value/1000);
      
      //creates the data to send to website
      String postData = "{\"moisture_level\": " + String(value) + "}";
    
      HTTPClient http;
      WiFiClient wifiClient;
      http.begin(wifiClient, serverName);
      http.addHeader("Content-Type", "application/json");
    
      int httpCode = http.POST(postData);

      if(httpCode > 0){
        if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
        }
        else{
          Serial.printf("[HTTP] Get... code %d\n", httpCode);
        }

        http.end();
      }
    }
    if (temp == HIGH){
        sendData();
        delay(1000);
    }
  }
  else{
    count = 0;
    if (temp == HIGH){
        sendData();
        delay(1000);
    }
  }

}


void sendData(){
  Serial.print("\nMOISTURE LEVEL : ");
      value= analogRead(sense_Pin);
      value= (value/1000);
      

      //creates the data to send to website
      String postData = "{\"moisture_level\": " + String(value) + "}";
    
      HTTPClient http;
      WiFiClient wifiClient;
      http.begin(wifiClient, serverName);
      http.addHeader("Content-Type", "application/json");
    
      int httpCode = http.POST(postData);

      if(httpCode > 0){
        if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
        }
        else{
          Serial.printf("[HTTP] Get... code %d\n", httpCode);
        }

        http.end();
      }
}
