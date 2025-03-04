const int pir1       = 5;    // PIR Sensor 1
const int pir2       = 4;    // PIR Sensor 2

const int motorIn1   = 25;   // L298N IN1
const int motorIn2   = 26;   // L298N IN2
const int motorEnA   = 27;   // L298N ENA (PWM)

const int potPin     = 36;   // Potentiometer

// Parameters
unsigned long runMotorDuration = 120000; // Run motor for 2 minutes after last new motion
unsigned long motorStopTime    = 0;      // When to stop the motor
bool motorRunning              = false;

// For ESP32 LEDC PWM
const int pwmChannel    = 0;    // choose any free channel (0-15)
const int pwmFrequency  = 5000; // 5 kHz
const int pwmResolution = 8;    // 8-bit duty (0-255)

// Debounce variables for each sensor
unsigned long lastTriggerTime1 = 0;
unsigned long lastTriggerTime2 = 0;
const unsigned long debounceDelay = 500; // 500 ms debounce period

void setup() {
  Serial.begin(9600);

  // PIR sensor inputs (assuming simple trigger mode where HIGH means motion)
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);

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
  unsigned long now = millis();
  bool newMotionDetected = false;

  // Check PIR Sensor 1 with debounce
  if (digitalRead(pir1) == HIGH) {
    if (now - lastTriggerTime1 > debounceDelay) {
      lastTriggerTime1 = now;
      newMotionDetected = true;
      Serial.println("PIR Sensor 1 detected motion!");
    }
  }

  // Check PIR Sensor 2 with debounce
  if (digitalRead(pir2) == HIGH) {
    if (now - lastTriggerTime2 > debounceDelay) {
      lastTriggerTime2 = now;
      newMotionDetected = true;
      Serial.println("PIR Sensor 2 detected motion!");
    }
  }

  // If new motion is detected, reset the shutdown timer
  if (newMotionDetected) {
    motorStopTime = now + runMotorDuration;
    if (!motorRunning) {
      motorRunning = true;
      // Set motor direction (forward)
      digitalWrite(motorIn1, HIGH);
      digitalWrite(motorIn2, LOW);
      Serial.println("Motor STARTED, timer set for 2 minutes.");
    } else {
      Serial.println("New motion detected, timer reset for 2 minutes.");
    }
  }

  // Read potentiometer to control motor speed
  int potValue = analogRead(potPin);   // Range: 0-4095 on ESP32
  int dutyCycle = map(potValue, 0, 4095, 0, 255);

  if (motorRunning) {
    ledcWrite(pwmChannel, dutyCycle);
  } else {
    ledcWrite(pwmChannel, 0);
  }

  // Stop the motor if no new motion has been detected for the full duration
  if (motorRunning && now >= motorStopTime) {
    motorRunning = false;
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    ledcWrite(pwmChannel, 0);
    Serial.println("Motor STOPPED after 2 minutes of no new motion.");
  }

  // Optional tiny delay to ease CPU usage
  // delay(10);
}
