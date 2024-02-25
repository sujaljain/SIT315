const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  Serial.begin(9600);
  startTimer();
}

void loop()
{
}

void startTimer()
{
  // Setting up Timer1 for interrupt-based blinking
  noInterrupts();
  TCCR1A = 0; // Clearing Timer1 control registers
  TCCR1B = 0;
  TCNT1 = 0; // Initializing counter value
  OCR1A = 31249; // Setting compare match value for 2-second interval
  TCCR1B |= (1 << WGM12); // Enabling CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Setting prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enabling Timer1 compare match interrupt
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); // Toggle LED state
}
