const int pir1 = 5; // PIR Sensor 1 connected to GPIO5
const int pir2 = 4; // PIR Sensor 2 connected to GPIO4
const int pir3 = 15; // PIR Sensor 3 connected to GPIO15

const int motorIn1 = 25; // L298N IN1
const int motorIn2 = 26;  // L298N IN2

int state = 0; // 0 = Monitoring, 1 = Motion detected
bool motionTriggered = false; // Variable to track if motion has already been detected

void setup() {
    Serial.begin(9600);
    pinMode(pir1, INPUT_PULLDOWN);
    pinMode(pir2, INPUT_PULLDOWN);
    pinMode(pir3, INPUT_PULLDOWN);

    pinMode(motorIn1, OUTPUT);
    pinMode(motorIn2, OUTPUT);
}

unsigned long motorStartTime = 0;
const long motorRunTime = 5000; // 5 seconds

void loop() {
    // Check motion sensors
    bool motion1 = digitalRead(pir1) == HIGH;
    bool motion2 = digitalRead(pir2) == HIGH;
    bool motion3 = digitalRead(pir3) == HIGH;
    
    // Current state
    unsigned long currentTime = millis();
    
    // If any PIR sensor detects motion and we're not already running the motor
    if ((motion1 || motion2 || motion3) && state == 0) {
        motorStartTime = currentTime;
        state = 1; // Set state to motion detected
        
        // Run the motor
        Serial.println("Motor running...");
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn2, LOW);
    }
    
    // If motor has been running for the specified time
    if (state == 1 && (currentTime - motorStartTime >= motorRunTime)) {
        // Stop the motor
        Serial.println("Motor stopped.");
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn2, LOW);
        
        state = 2; // Cooldown state
        motorStartTime = currentTime; // Reset timer for cooldown
    }
    
    // Reset after cooldown period (1 second)
    if (state == 2 && (currentTime - motorStartTime >= 1000)) {
        state = 0;
        Serial.println("Monitoring...");
    }
}
