const int motionSensorPin = 2;

void setup() {
    Serial.begin(9600);
  
    // Set the motion sensor pin as input
    pinMode(motionSensorPin, INPUT);
  
    // Initialize the built-in LED pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    int sensorState = digitalRead(motionSensorPin);

    if (sensorState == HIGH) {
        // Motion detected
        Serial.println("Motion detected!");
        digitalWrite(LED_BUILTIN, HIGH);  // Turn on the built-in LED
    } else {
        // No motion detected
        Serial.println("No motion.");
        digitalWrite(LED_BUILTIN, LOW);   // Turn off the built-in LED
    }

    // Wait for a short period before reading again
    delay(200);
}
