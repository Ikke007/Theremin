int fsrPin0 = 0;     // the FSR and 10K pulldown are connected to a0
int fsrPin1 = 1;
double fsrReading;     // the analog reading from the FSR resistor divider

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  analogReadResolution(12);
}

void loop(void) {
  
  fsrReading = analogRead(fsrPin0);
  Serial.print(fsrReading);  // the raw analog reading
  Serial.print(",");
  
  fsrReading = analogRead(fsrPin1);
  Serial.print(fsrReading);  // the raw analog reading
  Serial.print(",");

  Serial.flush();
  delay(20);
}
