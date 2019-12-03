unsigned long a = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  Serial.println("hello");
  delay(500);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  Serial.println(a);
  a = a + 1;
}

