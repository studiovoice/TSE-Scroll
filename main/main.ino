const int pir1       = 5;    // PIR Sensor 1
const int pir2       = 4;    // PIR Sensor 2
const int pir3       = 15;   // PIR Sensor 3

const int motorIn1   = 25;   // L298N IN1
const int motorIn2   = 26;   // L298N IN2
const int motorEnA   = 27;   // L298N ENA (PWM)

const int potPin     = 36;   // Potentiometer

// Parameters
unsigned long runMotorDuration = 3000; // Run motor for 3 seconds (placeholder, will change this to as long as there is motion)
unsigned long motorStopTime    = 0;    // When to stop the motor
bool motorRunning              = false;

// For ESP32 LEDC PWM
const int pwmChannel    = 0; // choose any free channel (0-15)
const int pwmFrequency  = 5000; // 5 kHz
const int pwmResolution = 8;    // 8-bit duty (0-255)

void setup() {
  Serial.begin(9600);

  // PIR inputs
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);

  // Motor driver pins
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);

  // Setup PWM for motor enable pin using LEDC
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(motorEnA, pwmChannel);

  // Ensure motor is initially off
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  ledcWrite(pwmChannel, 0);
}

void loop() {
  bool motion1 = (digitalRead(pir1) == HIGH);
  bool motion2 = (digitalRead(pir2) == HIGH);
  bool motion3 = (digitalRead(pir3) == HIGH);

  // Print which sensor(s) triggered
  if (motion1) Serial.println("PIR Sensor 1 detected motion!");
  if (motion2) Serial.println("PIR Sensor 2 detected motion!");
  if (motion3) Serial.println("PIR Sensor 3 detected motion!");

  if ((motion1 || motion2 || motion3) && (!motorRunning)) {
    motorRunning = true;
    motorStopTime = millis() + runMotorDuration;

    // Set direction (forward)
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);

    Serial.println("Motor STARTED for 3 seconds...");
  }

  int potValue = analogRead(potPin);   // range: 0-4095 on ESP32
  int dutyCycle = map(potValue, 0, 4095, 0, 255);

  // If the motor is running, apply the duty cycle
  if (motorRunning) {
    ledcWrite(pwmChannel, dutyCycle);
  } else {
    // If the motor is not running, set duty to 0
    ledcWrite(pwmChannel, 0);
  }

  if (motorRunning && (millis() >= motorStopTime)) {
    // Stop the motor
    motorRunning = false;
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    ledcWrite(pwmChannel, 0);

    Serial.println("Motor STOPPED after 3 seconds.");
  }

  // Optional tiny delay to ease CPU usage
  // delay(10);
}
