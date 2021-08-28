#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "mbedtls/md.h"
#include <Preferences.h> // Preferences Library
#include <nvs_flash.h>

Preferences devPreferences;
HTTPClient httpClient;
int ledPin = 2;

String host = "http://192.168.1.8/Esp32Initialize/initializeEspAudio?macId=";

String projStr = "SURESH";
String ssidStr = "ashwin_hall";
String passStr = "ashwin691";

String serialNumberStr = "";
String activationCodeStr = "";
String awsCodeStr = "";
String mobCodeStr = "";
String securityStr = "";

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  char ssid[] = "ashwin_hall";
  char pass[] = "ashwin691";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Waiting for WiFi");
  int printCount = 0;
  int ledStatus = 0; // 0 = OFF; 1 = ON
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (ledStatus == 0)
    {
      ledStatus = 1;
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);
    }
    printCount++;
    if (printCount > 50)
    {
      printCount = 0;
      Serial.println();
    }
  }
  digitalWrite(ledPin, HIGH);

  //Get Mac ID
  String macID = WiFi.macAddress();
  host = host + macID;
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  // const int httpPort = 80;
  //http://192.168.1.8/Esp32Initialize/initilizeEspAudio?macId=B4:E6:2D:8D:73:C1
  httpClient.begin(host);
  delay(100);
  int httpCode = httpClient.GET();
  // Serial.print("Received HTTP Code : ");
  // Serial.println(httpCode);

  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = httpClient.getString();
      delay(100);
      // Serial.println(payload);
      serialNumberStr = payload.substring(0, 10);
      activationCodeStr = payload.substring(10, 20);
      mobCodeStr = payload.substring(20, 32);
      awsCodeStr = payload.substring(32, 44);
      securityStr = payload.substring(44, 364);
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
  }

  /* Clear all memory */

  nvs_flash_erase();                              // erase the NVS partition and...
  nvs_flash_init();                               // initialize the NVS partition.
  devPreferences.begin("sureshkaval@100", false); //create Partion
  devPreferences.clear();

  //Clear partion
  /* TESTING if Partion is erased!!! */

  // Serial.println("BEFORE Project from memory = " + devPreferences.getString("project"));
  // Serial.println("BEFORE Serial Number from memory = " + devPreferences.getString("serialNumber"));
  // Serial.println("BEFORE Activation Code from memory = " + devPreferences.getString("activationCode"));
  // Serial.println("BEFORE SSID from memory = " + devPreferences.getString("ssid"));
  // Serial.println("BEFORE PASSWORD from memory = " + devPreferences.getString("pass"));
  // Serial.println("BEFORE SHA Digest loaded from Memory = " + devPreferences.getString("securityCode"));

  devPreferences.putString("project", projStr);
  delay(100);
  devPreferences.putString("ssid", ssidStr);
  delay(100);
  devPreferences.putString("pass", passStr);
  delay(100);
  devPreferences.putString("serialNumber", serialNumberStr);
  delay(100);
  devPreferences.putString("activationCode", activationCodeStr);
  delay(100);
  // devPreferences.putString("awsCode", awsCodeStr);
  // delay(100);
  // devPreferences.putString("mobCode", mobCodeStr);
  // delay(100);
  devPreferences.putString("securityCode", securityStr);
  delay(100);
  devPreferences.end();

  Serial.println("SERIAL NUMBER   = " + serialNumberStr);
  Serial.println("ACTIVATION CODE = " + activationCodeStr);
  Serial.println("AWS CODE        = " + awsCodeStr);
  Serial.println("MOB CODE        = " + mobCodeStr);
  Serial.println("SECURITY CODE   = " + securityStr);
}

void loop()
{
  // put your main code here, to run repeatedly:
}