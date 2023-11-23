#define READ_PIN 7

bool prv_state = 0;
unsigned long t = 0;
int count = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(READ_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int current_state = digitalRead(READ_PIN);
  if (prv_state == 0 && current_state == 1)
  {
    t = millis();
  }
  else if (prv_state == 1 && current_state == 0)
  {
    unsigned long duration = millis() - t;
    if (duration > 5000)
    {
      Serial.print("Number of pulse: ");
      Serial.println(count);
      count = 0;
    }
    else
    {
      count++;
    }
    // Serial.print("Duration: ");
    // Serial.println(duration);
  }
  
  prv_state = current_state;
}
