/*
  Web client

 This sketch connects to a website 
 using Wi-Fi functionality on MediaTek LinkIt platform.

 Change the macro WIFI_AP, WIFI_PASSWORD, WIFI_AUTH and SITE_URL accordingly.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 20 Aug 2014
 by MediaTek Inc.
 */

#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

#define WIFI_AP "krupa"
#define WIFI_PASSWORD "test@786"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your WiFi AP configuration
#define SITE_URL "api.mediatek.com"

LWiFiClient c;
int sensorPin = A1; 
int m = 0;  // variable to store the value coming from the sensor


void setup()
{
  LTask.begin();
  LWiFi.begin();
  Serial.begin(115200);
while(!Serial)
{
 ; 
}
dht.begin();

  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(1000);
  }
/*
  // keep retrying until connected to website
  Serial.println("Connecting to WebSite");
  while (0 == c.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }

  // send HTTP request, ends with 2 CR/LF
  Serial.println("send HTTP POST request");

  float h = 30.9;
  float t = 24.2;
  int m = 42;
  String data = "humidity,," + String(h) + "\n" + "temperature,," + String(t) + "\n" + "moisture,," + String(m);
  Serial.println(data);
  c.println("POST /mcs/v2/devices/DaDxjiy6/datapoints.csv HTTP/1.1");
  c.println("Host: " SITE_URL);
  int dataLength = data.length();

c.println("deviceKey: wjsFz1SQ8VdzPX5B");
c.print("Content-Length: ");
c.println(dataLength);
c.println("Content-Type: text/csv");
c.println("Connection: close");
c.println();
c.println(data);

  // waiting for server response
  Serial.println("waiting HTTP response:");
  while (!c.available())
  {
    delay(100);
  }*/
}

boolean disconnectedMsg = false;

void loop()
{
  
  SensorDataRead();
  // Make sure we are connected, and dump the response content to Serial
  while (c)
  {
    int v = c.read();
    if (v != -1)
    {
      Serial.print((char)v);
    }
    else
    {
      Serial.println("no more content, disconnect");
      c.stop();
      while (1)
      {
        delay(1);
      }
    }
  }

  if (!disconnectedMsg)
  {
    Serial.println("disconnected by server");
    disconnectedMsg = true;
  }
  delay(500);
}

void SensorDataRead()
{
  float t = 0.0;
    float h = 0.0;
     m = analogRead(sensorPin);
     
    // delay(1000);
    
    if(dht.readHT(&t, &h))
    {
       Serial.println("------------------------------");
        Serial.print("temperature = ");
        Serial.println(t);
        
        Serial.print("humidity = ");
        Serial.println(h);
        Serial.print("Soil Moisture Level :");
        Serial.println(m);
    }
    
    SensorDataPost(h, t, m);
    delay(2000); 
}

void SensorDataPost(float h, float t, int m)
{
 String data = "humidity,," + String(h) + "\n" + "temperature,," + String(t) + "\n" + "moisture,," + String(m);
  Serial.println(data);
  
  while (0 == c.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }
  c.println("POST /mcs/v2/devices/DaDxjiy6/datapoints.csv HTTP/1.1");
  c.println("Host: " SITE_URL);
  int dataLength = data.length();

c.println("deviceKey: wjsFz1SQ8VdzPX5B");
c.print("Content-Length: ");
c.println(dataLength);
c.println("Content-Type: text/csv");
c.println("Connection: close");
c.println();
c.println(data);

  // waiting for server response
  Serial.println("waiting HTTP response:");
  while (!c.available())
  {
    delay(100);
  } 
}
