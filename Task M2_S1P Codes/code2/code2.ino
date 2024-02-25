const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  Serial.begin(9600);
  // Starting the timer with a custom frequency (e.g., 2 Hz)
  startTimer(2.0); // Changing frequency as needed
}

void loop()
{
}

void startTimer(double timerFrequency)
{
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
