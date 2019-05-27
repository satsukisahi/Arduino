unsigned long a = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  Serial.println("hello");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  Serial.println(a);
  a = a + 1;
}
