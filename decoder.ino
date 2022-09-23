//A quick and dirty Arduino sketch to decode the signal sent by a rotary dial.
//This is basically a FSM that counts the pulses and sends the corresponding number through serial.

long WAIT_TIME=150; //How long to wait for another pulse to come in (in milliseconds)
long MIN_PULSE_LENGTH=10; //How long to wait before checking for another pulse (in milliseconds)
int DIAL_PIN=2; //Which pin is connected to the rotary dial

enum states {IDLE, PULSE_IN, WAITING, SENDING};
states STATE;
int n;
long start_time;

void setup() {
  pinMode(DIAL_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  STATE = IDLE;
  n=0;
}

void loop() {
  switch (STATE) {
    case IDLE:
      n = 0;
      if (digitalRead(2) == HIGH){
        n += 1;
        STATE = PULSE_IN;
      }
    break;
    case PULSE_IN:
      delay(MIN_PULSE_LENGTH);
      if (digitalRead(2) == LOW){
        start_time = millis();
        STATE = WAITING;
      }
    break;
    case WAITING:
      if ((millis() - start_time) < WAIT_TIME){
        if (digitalRead(2) == HIGH){
          n += 1;
          STATE = PULSE_IN;
        }
      } else {
        STATE = SENDING;
      }
    break;
    case SENDING:
      n %= 10;
      Serial.println(n);
      STATE = IDLE;
    break;
  }
}
