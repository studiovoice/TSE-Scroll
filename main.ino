const int pir1 = 5; // PIR Sensor 1 connected to GPIO5
const int pir2 = 4; // PIR Sensor 2 connected to GPIO4
const int pir3 = 2; // PIR Sensor 3 connected to GPIO2

int state = 0; // 0 = Monitoring, 1 = Motion detected
bool motionTriggered = false; // Variable to track if motion has already been detected

void setup() {
    Serial.begin(9600);
    pinMode(pir1, INPUT_PULLDOWN);
    pinMode(pir2, INPUT_PULLDOWN);
    pinMode(pir3, INPUT_PULLDOWN);
}

void loop() {
    bool motion1 = digitalRead(pir1) == HIGH;
    bool motion2 = digitalRead(pir2) == HIGH;
    bool motion3 = digitalRead(pir3) == HIGH;

    // If any PIR sensor detects motion, but only trigger if no motion has been detected yet
    if ((motion1 || motion2 || motion3) && !motionTriggered) {
        motionTriggered = true; // Mark motion as triggered
        if (motion1) Serial.println("Motion detected by PIR Sensor 1!");
        if (motion2) Serial.println("Motion detected by PIR Sensor 2!");
        if (motion3) Serial.println("Motion detected by PIR Sensor 3!");
        state = 1; // Set state to motion detected
    } 
    // If no motion detected from any sensor
    else if (!motion1 && !motion2 && !motion3) {
        if (state != 0) { // Print only when state changes
            Serial.println("Monitoring...");
            state = 0; // Reset state to monitoring
            motionTriggered = false; // Reset motion triggered flag
        }
    }

    // delay(100); // Small delay for stability (optional)
}

