#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

bool brokenHouse = false;
bool doorbell = false;

int status = WL_IDLE_STATUS; //not required.
int data;
const char* ssid = "TP-Link_1DD0";
const char* password = "92921504";


void setup()
{
  Serial.begin(9600);
  s.begin(9600);
  Serial.println();

  Serial.println("try");
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
    if (s.available()>0)
    {
      data=s.read();
      Serial.print(data);
      if(data == 1) //wrong code
        {
          if (get_http(String("wrong_code")) != 0);
        }
      else if(data == 2)
        {
          if (get_httpDoorbell(String("doorbell")) != 0);
        }
    }
}

int get_http(String state)
{
    HTTPClient http;
    WiFiClient client;
    int ret = 0;
    
    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://maker.ifttt.com/trigger/wrong_code/with/key/d91H3yPsQ1EaKTcShhQbqr");
   
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
   
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

    if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}

int get_httpDoorbell(String state)
{
    HTTPClient http;
    WiFiClient client;
    int ret = 0;
    
    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://maker.ifttt.com/trigger/doorbell/with/key/d91H3yPsQ1EaKTcShhQbqr");
   
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
   
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

    if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}
