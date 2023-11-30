#define LED_PIN 6
#define DATA_SIZE 8
#define BIT_0_DELAY 2000
#define BIT_1_DELAY 1000
#define CLOCK_DELAY 100
#define SYNC_DELAY 500
// #define SYNC_PATTERN 0b00000000 // Example sync pattern


// Store binary data as byte type
byte sync[] = {0, 0, 0, 0, 0, 0, 0, 0};
byte data[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool lastStateHigh = false; 

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  delay(1000);
  sendStartPulse();
  fmmod();
}

void sendStartPulse() {
  // Keep track of previous state
  Serial.println("Start sending starting pulse");
  lastStateHigh = !lastStateHigh;
  digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW);
  delay(3000); // Starting pulse
  Serial.println("Ending starting pulse");
} 

void transmitBit(byte bitVal) {
  
  // Start every bit with a signal to represent the clock pulse
  //digitalWrite(LED_PIN, HIGH);
  //delay(CLOCK_DELAY); // Short delay for clock pulse
  
  if (bitVal == 0) {
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    // For '0', for the whole bit period
    digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW);
    // Delay for the entire bit period
    delay(BIT_0_DELAY); 
  } else {
    // For '1', change mid-way through the bit period
    // First half
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    digitalWrite(LED_PIN, lastStateHigh ? LOW : HIGH);  
    delay(BIT_1_DELAY);
    // Second half 
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    digitalWrite(LED_PIN, lastStateHigh ? HIGH : LOW); 
    delay(BIT_1_DELAY);
  }
}

void syncbit(byte syncval) {

  if (syncval == 0) {
    // For '0'
    digitalWrite(LED_PIN, LOW);
    delay(SYNC_DELAY); 
  } else {
    // For '1'
    digitalWrite(LED_PIN, HIGH);  
    delay(SYNC_DELAY);
  }
}
void fmmod() {
    // Transmit actual data
  for (int i = 0; i < DATA_SIZE; i++) {
    trasmitbit(sync[i]);
    Serial.println("sync");
  }
  // Transmit actual data
   for (int i = 0; i < DATA_SIZE; i++) {
     transmitBit(data[i]);
     Serial.println("data");
   }
  
}

void loop() {
  delay(5000); // Delay before repeating the sequence
}
