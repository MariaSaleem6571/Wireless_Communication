#define LED_PIN 6
#define N_BIT 8
//size of synchronous pulse in bit
#define SYNC_SIZE 8
//size of data pulse in byte
#define DATA_SIZE 8

#define BIT_0_DELAY 20
#define BIT_1_DELAY 10
#define STARTING_DELAY 80

//#define BIT_0_DELAY 500
//#define BIT_1_DELAY 250
//#define STARTING_DELAY 1000


String s = "";

int width = 64;
int height = 64;
const unsigned char image[] = {
0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xff, 0xff, 0xe0, 0x00, 0x00, 0x57, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x47, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x38, 0x43, 0xff, 0xff, 0xff, 0xff, 0xe9, 0x00, 0x70, 0x43, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x60, 0xf1, 0x43, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x43, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xf9, 0xf9, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xeb, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xf9, 0x9f, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x40, 0x07, 0xe7, 0xff, 0xff, 0xff, 0xff, 0x80, 0xe0, 0x03, 0xf7, 0xff, 0xff, 
0xff, 0xff, 0xe0, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0x01, 0xf7, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xfc, 0x01, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xf7, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfd, 0xff, 0xcf, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x80, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xf1, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xe3, 0xf0, 0xcf, 0xbf, 0xff, 0xff, 0x7f, 0xff, 0xe7, 0x20, 0xe7, 0x3f, 0xff, 0xff, 
0x7f, 0xff, 0xce, 0xc2, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc0, 0x0c, 0x3f, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
0xff, 0x80, 0x0f, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0x00, 0x37, 0xff, 0xff, 0xff, 0xf3, 0xff, 
0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf9, 0xff, 
0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x7f, 
0xf8, 0x00, 0x3f, 0xbe, 0xff, 0xff, 0xfc, 0x7f, 0xf0, 0x00, 0x3c, 0x02, 0xff, 0xff, 0xfc, 0x3f, 
0xf0, 0x00, 0x10, 0x02, 0x7f, 0xff, 0xfc, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x1f, 
0xf0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x0f, 0xe0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x0f, 
0xe0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x0f, 0x60, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x07
};

//const unsigned char image[] = {
//0x00, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00,
//0x10, 0xfe, 0x3f, 0x5f, 0xf2, 0xff, 0xff, 0x00,
//};


// Store binary data as byte type
byte sync = 0x00;
bool lastStateHigh = false; 

void sendStartPulse() {
  // Keep track of previous state
//  Serial.println("Start sending starting pulse");
  lastStateHigh = !lastStateHigh;
  digitalWrite(LED_PIN, lastStateHigh);
  delay(STARTING_DELAY); // Starting pulse
//  Serial.println("Ending starting pulse");
} 

void transmitBit(byte bitVal) 
{  
  if (bitVal == 0) {
    lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
    // For '0', for the whole bit period
    digitalWrite(LED_PIN, lastStateHigh);
//    Serial.println(0);
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
//    Serial.println(1);
    delay(BIT_1_DELAY);
  }
}

byte calculateChecksum(int im_idx) 
{
  byte checksum = 0x00;
  for (int i = 0; i < N_BIT; i++) {
    // Extracting each bit and XOR-ing them together
    checksum ^= ((image[im_idx] >> i) & 0x01);
  }
  return checksum;
}

byte calculateTotalChecksum(int start_idx) 
{
  byte checksum = 0x00;
  for (int i = 0; i < DATA_SIZE; i++) 
  {
    checksum |= (calculateChecksum(start_idx + i) << i);
  }
  return checksum;
}

void sendSingleChunk(int start_idx)
{
  // 1 chunk contain 1 starting pulse, 1 byte sync pulse, 8 byte data, 1 byte checksum   
  sendStartPulse();
  // send sync pulse 
//  Serial.println("sync");
  for (int i = 0; i < SYNC_SIZE; i++) 
  {
    transmitBit((sync >> i) & 0x01);
  }
  // send data pulse
  for (int j = 0; j < DATA_SIZE; j++) 
  {
//    Serial.print("data ");
//    Serial.println(j+1);
    for (int i = 0; i < N_BIT; i++) 
    {
      transmitBit((image[start_idx+j] >> i) & 0x01);
    }
  }
//  Serial.println("sum");
  // send check sum  
  byte checksum = calculateTotalChecksum(start_idx);
  for (int i = 0; i < N_BIT; i++) 
  {
    transmitBit((checksum >> i) & 0x01);
  }
}

void setup() {
  Serial.begin(9600);
  for (int i=0;i<sizeof(image)/sizeof(byte);i+=N_BIT)
  {
    sendSingleChunk(i);
  }
  lastStateHigh = !lastStateHigh; // Toggle the last state for the next bit
  digitalWrite(LED_PIN, lastStateHigh);  
}

void loop() {
  
}
