// Define the pin connected to the motion sensor
const int motionSensorPin = 11;
const int pushButton = 2;
int LED = LOW;

void setup() {
    Serial.begin(9600);
  
    // Set the motion sensor pin as input
    pinMode(motionSensorPin, INPUT);
  
    // Initialize the built-in LED pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
  
    // Initialising the button input
    pinMode(pushButton, INPUT_PULLUP);
  
    // Interrupt
    attachInterrupt(digitalPinToInterrupt(pushButton), InterruptFunc, CHANGE);
}

void loop() {
    int sensorState = digitalRead(motionSensorPin);

    if (sensorState == HIGH) {
        // Motion detected
        Serial.println("Motion detected!");
        digitalWrite(LED_BUILTIN, HIGH);  // Turn on the built-in LED
      LED = HIGH;
    } else {
        // No motion detected
        Serial.println("No motion.");
        digitalWrite(LED_BUILTIN, LOW);   // Turn off the built-in LED
      LED = LOW;
    }

    // Wait for a short period before reading again
    delay(200);
}

void InterruptFunc() {
  // Toggle the LED state
  digitalWrite(LED_BUILTIN, !LED);
  Serial.println("Interrupt Occured!");
  delay(200);
}
