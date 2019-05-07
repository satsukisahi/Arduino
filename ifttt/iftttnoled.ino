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
#define MSEC2CLOCK(ms)    (ms * 80000L)
#define JST     3600*9
String LCD_STRING;
char s[8];
double SET;//変数の宣言
const char* host = "mqtt.beebotte.com";

bool onbed=0;
int sec;
bool ontime;
int artime=30000;
int lastar=-87400;
bool lcd=1;

// メッセージを受け取ったらシリアルにプリント
void callback(char* topic, byte* payload, unsigned int length) {
 
  // PubSubClient.hで定義されているMQTTの最大パケットサイズ
  char buffer[MQTT_MAX_PACKET_SIZE];

  snprintf(buffer, sizeof(buffer), "%s", payload);
  /*   Serial.println("received:");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.println(buffer); */

  // 受け取ったJSON形式のペイロードをデコードする
  StaticJsonBuffer<MQTT_MAX_PACKET_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(buffer);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  const char* parsedPayload = root["data"];
  if (parsedPayload != NULL) {
    Serial.print("payload: ");
    Serial.println(parsedPayload);

    if(strcmp(parsedPayload, "on") == 0){
      onbed=1;
    }
    else if(strcmp(parsedPayload, "off") == 0){
      onbed=0;
    }
    else{
      int b = atoi(parsedPayload);
      if(b!=0)artime=b;
      LCD_STRING=String((int)artime/3600)+':'+String(((int)artime%3600)/60);
      LCD_DISP_8_2();
    }
  }
}

WiFiClient wifiClient;
PubSubClient client(host, 1883, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(13, OUTPUT);
  configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timer0_ISR);
  timer0_write(ESP.getCycleCount() + MSEC2CLOCK(5000) ); // 5000msec
  interrupts();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      // Wi-Fiアクスポイントへの接続に失敗したら5秒間待ってリトライ
      Serial.println("failed to connect");
      delay(5000);
      return;
    } else {
      Serial.print("WiFi connected: ");
      Serial.println(WiFi.localIP());
    }
  }

  // クライアントがサーバに接続されていなければ
  if (!client.connected()) {
    // ユーザ名を指定して接続
    String username = "token:";
    username += channelToken;
    client.connect(clientID, username.c_str(), NULL);

    if (client.connected()) {
      Serial.println("MQTT connected");
      client.setCallback(callback);

      // トピック名を指定してsubscribe
      client.subscribe(topic);
    } else {
      Serial.print("MQTT connection failed: ");
      Serial.println(client.state());
      delay(5000);
    }
  } else {
    // 既にサーバに接続されていれば通常処理を行う
    client.loop();
  }
}




//5000ms毎にここが呼び出される
void timer0_ISR (void) {

  if(lcd){
    Wire.begin(12,14);
    delay(2);
    writeCommand(0x38);
    delay(2);
    writeCommand(0x39);
    delay(2); 
    writeCommand(0x14);
    delay(2);
    writeCommand(0x73);
    delay(2);//3.3V=0x73, 5V=0x7A(コントラスト調整しました)
    writeCommand(0x56);
    delay(2);//3.3V=0x56, 5V=0x54
    writeCommand(0x6C);
    delay(2);
    writeCommand(0x38);
    delay(2);
    writeCommand(0x01);
    delay(2);
    writeCommand(0x0C);
    delay(2);
    LCD_STRING=String((int)artime/3600)+':'+String(((int)artime%3600)/60);
    LCD_DISP_8_2();
    lcd=0;
  }



  timer0_write(ESP.getCycleCount() + MSEC2CLOCK(5000) ); 
  // TODO 割り込み処理をここに
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  /*   Serial.printf("%02d:%02d:%02d\n",
  tm->tm_hour, tm->tm_min, tm->tm_sec); */
  sec=int(tm->tm_hour)*3600+int(tm->tm_min)*60+int(tm->tm_sec);
  if(sec>87300){
    lastar=-87400;
  }
  Serial.print(sec);
  Serial.print(" ");
  Serial.println(artime);
  if(artime<sec&&sec<72000){
    ontime=1;
  }
  else if(sec<lastar+18000&&sec<72000){
    ontime=1;
  }
  else{
    ontime=0;
  }
  if(onbed&&ontime){
    digitalWrite(13, HIGH);   // set the LED on
    lastar=sec;
  }
  else{
    digitalWrite(13, LOW);    // set the LED off
  }
}


//コマンド書き込み
void writeCommand(byte command){
    Wire.beginTransmission(0x3e);
    Wire.write(0x00);
    Wire.write(command);
    Wire.endTransmission();
    delay(10);
}
//データ書き込み
void writeData(byte data){
    Wire.beginTransmission(0x3e);
    Wire.write(0x40);
    Wire.write(data);
    Wire.endTransmission();
    delay(1);
}
//LCDの1行目にデータ書き込み
void LCD_DISP_8_1(void){
    writeCommand(0x80);//DDRAMアドレスを2行目先頭にセット
    for(int i = 0; i < 8; i++){writeData(LCD_STRING[i]);}
}
//LCDの2行目にデータ書き込み
void LCD_DISP_8_2(void){
    writeCommand(0x40+0x80);//DDRAMアドレスを2行目先頭にセット
    for(int i = 0; i < 8; i++){writeData(LCD_STRING[i]);}
}