#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "Air2.4";        //  *** 書き換え必要 ***
const char *pass = "tsu0354526227";    //  *** 書き換え必要（8文字以上）***
ESP8266WebServer Server(80);         //  ポート番号（HTTP）
int Counter = 0;                     //  お客さんカウンタ

//  クライアントにウェブページ（HTML）を返す関数
void handleRoot() {
  //  レスポンス文字列の生成（'\n' は改行; '\' は行継続）
  Counter++;
  Serial.println("handleRoot()");
  String message = "\
<html lang=\"ja\">\n\
<meta charset=\"utf-8\">\n\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
<head>\n\
  <title>トンペイーノ</title>\n\
</head>\n\
<body style=\"font-family: sans-serif; background-color: #ffeeaa;\" >\n\
  <h1>トンペイーノによるウェブサーバ</h1>\n\
  <h3>あなたはトンペイーノの "
  + String(Counter)
  + "人目のお客さまです！</h3>\n\
</body>\n\
</html>\n";
  //  クライアントにレスポンスを返す
  Server.send(200, "text/html", message);  
}

//  クライアントにエラーメッセージを返す関数
void handleNotFound() {
  //  ファイルが見つかりません
  Serial.println("handleNotFound()");
  Server.send(404, "text/plain", "File not found in Dongbeino...");
}

//  メインプログラム
void setup() {
  //  シリアルモニタ（動作ログ）
  Serial.begin(115200);               //  ESP 標準の通信速度 115200
  delay(100);                         //  100ms ほど待ってからログ出力可
  Serial.println("\n*** Dongbeino ***");
  //  無線 LAN に接続
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);             //  接続確立まで待つこと
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());     //  ESP 自身の IP アドレスをログ出力
  //  ウェブサーバの設定
  Server.on("/", handleRoot);         //  ルートアクセス時の応答関数を設定
  Server.onNotFound(handleNotFound);  //  不正アクセス時の応答関数を設定
  Server.begin();                     //  ウェブサーバ開始
}
void loop() {
  //  クライアントからの要求を処理する
  Server.handleClient();
}