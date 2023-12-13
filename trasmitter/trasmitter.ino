#define LED_PIN 6
#define DATA_SIZE 8
#define BIT_0_DELAY 10
#define BIT_1_DELAY 5
#define STARTING_DELAY 100


// Store binary data as byte type
byte sync[] = {0, 0, 0, 0, 0, 0, 0, 0};
byte data[] = {0, 1, 0, 1, 1, 1, 1, 1};
byte check_sum = 0;
bool lastStateHigh = false; 

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  delay(STARTING_DELAY);
  sendStartPulse();
  fmmod(1);
  checkSum();
}

void sendStartPulse() {
  // Keep track of previous state
  Serial.println("Start sending starting pulse");
  lastStateHigh = !lastStateHigh;
  digitalWrite(LED_PIN, lastStateHigh);
  delay(STARTING_DELAY); // Starting pulse
  Serial.println("Ending starting pulse");
} 

void transmitBit(byte bitVal) {
  
  if (bitVal == 0) {
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    // For '0', for the whole bit period
    digitalWrite(LED_PIN, lastStateHigh);
    // Delay for the entire bit period
    delay(BIT_0_DELAY); 
  } else {
    // For '1', change mid-way through the bit period
    // First half
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    digitalWrite(LED_PIN, lastStateHigh);  
    delay(BIT_1_DELAY);
    // Second half 
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    digitalWrite(LED_PIN, lastStateHigh); 
    delay(BIT_1_DELAY);
  }
}
void fmmod(bool is_check_sum) {
  if (is_check_sum)
  {
    // calculate check sum before to reduce delay    
    checkSum();
  }
  // Transmit actual data
  for (int i = 0; i < DATA_SIZE; i++) {
    transmitBit(sync[i]);
//    Serial.println("sync");
  }
  // Transmit actual data
  for (int i = 0; i < DATA_SIZE; i++) {
    transmitBit(data[i]);
//    Serial.println("data");
  }
  if (is_check_sum)
  {
    transmitBit(check_sum); 
  }
  lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
  digitalWrite(LED_PIN, lastStateHigh);  
}
void checkSum()
{
  int sum = 0;
  for (int i = 0; i < DATA_SIZE; i++) 
  {
    sum += data[i];
  }
  check_sum = byte(sum % 2);
}

void loop() {
}
