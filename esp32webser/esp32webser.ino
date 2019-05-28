#include "simplewebserver.h"
 
// クライアントから/ledonをもらったら点灯し、ledoffなら消灯
// GPIO13にLEDを接続。
 
#define LED 13
SimpleWebServer server("ESP32AP", "password", IPAddress(192,168,4,1), IPAddress(255,255,255,0), 80);
void led_menu() {
  const char* cur = digitalRead(LED) ? "ON" : "OFF";
  String s =  "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n"
              "<HTML><BODY style='font-size:48px;'>ESP32_HTTPD_LED1<br/>"
              "<br/><a href=/ledon>ON</a><br/><a href=/ledoff>OFF</a><br/><br/>"
              "Current Status = ";
  s = s + String(cur) + "</BODY></HTML>\r\n";
  server.send_response(s.c_str());
}
 
void led_on()
{
  digitalWrite(LED, 1);
  led_menu();
}
 
void led_off()
{
  digitalWrite(LED, 0);
  led_menu();
}
 
void led_500()
{
  server.send_status(500, "Internal Server Error");
}
 
void default_handler(String request_line)
{
  Serial.println("default_handler : " + request_line);  
  String s = request_line + " is not found";
  server.send_status(404, s.c_str());
}
 
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
 
  server.add_handler("/", led_menu);
  server.add_handler("/ledon", led_on);
  server.add_handler("/ledoff", led_off);
  server.add_handler("/led500", led_500);
 
  server.add_default_handler(default_handler);
  server.begin();
}
 
void loop() {
  server.handle_request();
  delay(10);
}