const int pir1 = 5;   // PIR Sensor 1 connected to GPIO5
const int pir2 = 4;   // PIR Sensor 2 connected to GPIO4
const int pir3 = 15;  // PIR Sensor 3 connected to GPIO15

const int motorIn1 = 25;  // L298N IN1 connected to GPIO25
const int motorIn2 = 26;  // L298N IN2 connected to GPIO26

bool motionDetected = false;   // Track if motion was detected
unsigned long lastMotionTime = 0;  // Time when motion was last detected
const unsigned long stopDelay = 60000;  // 1 minute in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
}

void loop() {
  bool motion1 = digitalRead(pir1) == HIGH;
  bool motion2 = digitalRead(pir2) == HIGH;
  bool motion3 = digitalRead(pir3) == HIGH;
  bool motionNow = motion1 || motion2 || motion3;  // Check if any sensor detects motion

  if (motionNow) {
    if (motion1) Serial.println("Motion detected by PIR Sensor 1!");
    if (motion2) Serial.println("Motion detected by PIR Sensor 2!");
    if (motion3) Serial.println("Motion detected by PIR Sensor 3!");

    lastMotionTime = millis();  // Update the last motion time
    
    if (!motionDetected) {  // Only trigger motor start once
      Serial.println("Motor running...");
      digitalWrite(motorIn1, HIGH);
      digitalWrite(motorIn2, LOW);
      motionDetected = true; // Mark motion as detected
    }
  }

  // Check if we should stop the motor (no motion for 1 minute)
  if (motionDetected && !motionNow && (millis() - lastMotionTime >= stopDelay)) {
    Serial.println("No motion detected for 1 minute. Stopping motor...");
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    motionDetected = false;
  }
}