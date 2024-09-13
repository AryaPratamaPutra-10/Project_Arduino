

#define BLYNK_PRINT Serial
#include <ESP8266Wifi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "SHaeyZlMslqLEH4-ZoKsc_3SjGKyPRAB"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmalboro";
char pass[] = "123456789";

BLYNK_WRITE(V0) (
  digitalWrite(D4, param.asInt());
 )

 void setup() {
  pinMode(D4, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
 }

 void loop() {
 }

  
