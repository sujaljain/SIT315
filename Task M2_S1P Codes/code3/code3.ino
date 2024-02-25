const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  Serial.begin(9600);
  // Starting the timer with a custom frequency (read from potentiometer)
  startTimer();
}

void loop()
{
}

void startTimer()
{
  // Reading the analog value from the potentiometer
  int analogValue = analogRead(METER_PIN);
  
  // Mapping the analog value to a desired frequency range (e.g., 1 Hz to 10 Hz)
  double timerFrequency = map(analogValue, 0, 1023, 1, 10);
  
  // Calculating the compare match value (OCR1A) based on the desired frequency
  // Formula: OCR1A = (F_CPU / (prescaler * frequency)) - 1
  // Assuming F_CPU = 16 MHz and prescaler = 1024
  unsigned long ocrValue = (16000000UL / (1024UL * timerFrequency)) - 1;
  
  // Setting up Timer1 for interrupt-based blinking
  noInterrupts();
  TCCR1A = 0; // Clearing Timer1 control registers
  TCCR1B = 0;
  TCNT1 = 0; // Initializing counter value
  OCR1A = ocrValue; // Setting compare match value based on timerFrequency
  TCCR1B |= (1 << WGM12); // Enabling CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Setting prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enabling Timer1 compare match interrupt
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); // Toggle LED state
}
