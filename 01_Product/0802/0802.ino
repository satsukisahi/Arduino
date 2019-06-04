//NOBのArduino日記！_AE-AQM0802の使い方_(20180828)
#include <Wire.h>//関数読み込み
String LCD_STRING;
char s[8];
double SET;//変数の宣言

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

//液晶初期化
void setup() {
    Serial.begin(115200);
    Serial.println("hello");
    Wire.begin(12,14);
    delay(100);
    writeCommand(0x38);
    delay(20);
    writeCommand(0x39);
    delay(20); 
    writeCommand(0x14);
    delay(20);
    writeCommand(0x73);
    delay(20);//3.3V=0x73, 5V=0x7A(コントラスト調整しました)
    writeCommand(0x56);
    delay(20);//3.3V=0x56, 5V=0x54
    writeCommand(0x6C);
    delay(20);
    writeCommand(0x38);
    delay(20);
    writeCommand(0x01);
    delay(20);
    writeCommand(0x0C);
    delay(20);
    Serial.println("hello");
}

//無限ループ
void loop() {
    LCD_STRING=String("Nob"+String(dtostrf(random(-1000.0,1000.0)/10.0,5,1,s)));//乱数代入
    LCD_DISP_8_1();//LCD一行目に変数を出力する
    LCD_STRING = "Arduino!";//「NobArduinoDiary!」代入
    LCD_DISP_8_2();
    delay(1000);
    Serial.println("qa");//LCD二行目に変数を出力する
}