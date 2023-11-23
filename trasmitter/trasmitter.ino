#define LED_PIN 6
#define DATA_SIZE 8
#define BIT_0_DELAY 1000
#define BIT_1_DELAY 250
#define CLOCK_DELAY 100
#define SYNC_PATTERN 0b01111110 // Example sync pattern


// Store binary data as byte type
byte data[] = {0, 1, 1, 0, 1, 1, 1, 0};
//byte data[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool lastStateHigh = false; 

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void sendStartPulse() {
  lastStateHigh = !lastStateHigh;
  digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW);
  delay(5000); // Starting pulse
}

 // Keep track of previous state


void transmitBit(byte bitVal) {
  
  // Start every bit with a signal to represent the clock pulse
  //digitalWrite(LED_PIN, HIGH);
  //delay(CLOCK_DELAY); // Short delay for clock pulse
  
  if (bitVal == 0) {
    // For '0', for the whole bit period
    digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW);
    // Delay for the entire bit period
    delay(BIT_0_DELAY); 
  } else {
    // For '1', change mid-way through the bit period
    // First half
    digitalWrite(LED_PIN, lastStateHigh ? LOW : HIGH);  
    delay(BIT_1_DELAY);
    // Second half 
    digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW); 
    delay(BIT_1_DELAY);
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
  }
}



void fmmod() {
  
  // Transmit sync pattern first
  for (int i = 7; i >= 0; i--) { 
    // Transmit each bit of the sync pattern
    transmitBit((SYNC_PATTERN >> i) & 1);
  }

  // Transmit actual data
  for (int i = 0; i < DATA_SIZE; i++) {
    transmitBit(data[i]);
  }
}

void loop() {
  sendStartPulse();
  fmmod();
  delay(5000); // Delay before repeating the sequence
}
