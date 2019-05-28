#include <ESP8266WiFi.h>
#define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include <Wire.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
unsigned long a = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  Serial.println("hello");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
  Serial.println(a);
  a = a + 1;
}

