void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port  only
  }
}
void loop() { // run over and over 
  if (Serial.available()) {
    Serial.write(Serial.read());
  }
}
