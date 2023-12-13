#define READ_PIN 3
#define N_SYNC 8
#define N_BIT 8
#define N_DATA 8
#define N_SUM 1
#define STARTING_DELAY 80

bool current_state;
String sstate = "None"; 

// the variable to check that the current pulse is starting pulse
bool is_starting_pulse = 1;
bool is_sync_pulse = 0;
bool is_data_pulse = 0;

bool is_measure_start = 0;

unsigned long start_t = 0;
int signal_idx = 0;

//bool data[N_BIT + N_SUM];
byte data[512];
byte check_sum = 0x00;
int data_idx = 0;
int checksum_idx = 0;
int bit_idx = 0;
int one_data_counter = 0;

int trans_delay = 0;
int trans_i = 0;

int width = 16*8;
int height = 1;
String image = "";

void checkStart(int start_duration)
{
  if (is_measure_start == 0)
  {
    start_t = millis();
//    sstate = "Start Starting pulse";
    is_measure_start = 1;
  }
  // if starting is longer than min duration then it's a starting pulse instead of signal pulse
  else
  { 
    int _t = millis() - start_t;
    if (_t > 0.7*start_duration && _t < 1.3*start_duration)
    {
      is_starting_pulse = 0; 
      is_sync_pulse = 1;
      is_data_pulse = 0;
      
//      sstate = "Starting pulse correct";
      is_measure_start = 0;
      start_t = millis();
    }
    else
    {
      // measure again unitil correct      
//      sstate = "Start Starting pulse";
      is_measure_start = 1;
      start_t = millis();
    }
  }

}
void calculateSyncPulse()
{
  // calculate delay
  trans_i++; 
  int _t = millis() - start_t;
  // calculate average 
  trans_delay = (trans_delay*(trans_i-1) + _t)/(trans_i);
//  sstate = String(trans_delay);
  
  if (trans_i >= N_SYNC)
  {
      is_starting_pulse = 0; 
      is_sync_pulse = 0;
      is_data_pulse = 1;
      trans_i = 0;
//      sstate = "delay: " + String(trans_delay) + ", Move to data pulse";
  }
  start_t = millis();
}
void checkDataPulse()
{ 
  int duration = int(millis() - start_t);
  if (duration > 0.7*trans_delay)
  {
    if ((data_idx+1)%N_DATA == 0)
    {
      check_sum |= (0x00 << bit_idx);
    }
    else if (checksum_idx < N_SUM)
    {
      data[data_idx] |= (0x00 << bit_idx); 
    }
    bit_idx++;
  }
  else
  {
    one_data_counter++;
    if (one_data_counter >= 2)
    {
      if ((data_idx+1)%N_DATA == 0)
      {
        check_sum |= (0x01 << bit_idx);
      }
      else if (checksum_idx < N_SUM)
      {
        data[data_idx] |= (0x01 << bit_idx); 
      }
      bit_idx++;
      one_data_counter = 0;
    }
  }

  if (bit_idx >= N_BIT)
  {
    bit_idx = 0;
    if ((data_idx+1)%N_DATA == 0)
    {
//      showData(check_sum);
      checksum_idx++;
    }
    else if (checksum_idx < N_SUM)
    {
//      showData(data[data_idx]);
      data_idx++;
    }
  }
  if (checksum_idx > N_SUM)
  {  
    checksum_idx = 0;
    data_idx++;
    
    is_starting_pulse = 1; 
    is_sync_pulse = 0;
    is_data_pulse = 0;
    // show previous data   
    showTotalData(data_idx-N_DATA);

    check_sum = 0x00;
    is_measure_start = 1;
  }
  start_t = millis();
}

bool checkSum()
{
  int sum = 0;
  for(int i=0; i<N_BIT; i++)
  {
    sum += data[i];
  }
  if (sum%2 == data[N_BIT])
    return true;
  else
    return false;
}

void showData(byte data_i)
{
  sstate = "Data = ";
  for(int i=0; i<N_BIT; i++)
  {
    sstate += String((data_i >> (7-i)) & 0x01) + " "; 
  }
  sstate += ", end_idx: " + String(data_idx+N_DATA);
}
void showTotalData(int start_idx)
{
  sstate = "Data = ";
  for(int i=0; i<N_BIT; i++)
  {
    sstate += "0x" + String(data[start_idx + i], HEX) + " "; 
  }
  sstate += "start idx: " + String(start_idx);
}
void isrSig()
{
  if (is_starting_pulse)
  {
    checkStart(STARTING_DELAY);
  }
  else if (is_sync_pulse)
  {
    calculateSyncPulse();
  }
  else if (is_data_pulse)
  {
    checkDataPulse();
  }
}
void showImage()
{
  for(int im_idx=0; im_idx < sizeof(data)/sizeof(byte); im_idx++)
  {
    showOneChar(im_idx);
    if ((im_idx+1)%(width/8) == 0)
    {     
      Serial.println(image);
      image = "";
    }
  }
}
void showOneChar(int im_idx)
{
  for(int i=0;i<8;i++)
  {
    if ((data[im_idx] >> (7-i)) & 0x01)
      image += " ";
    else
      image += "*";
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  digitalWrite(READ_PIN, HIGH);
  pinMode(READ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(READ_PIN), isrSig, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(sstate);  
  if (data_idx >= 512)
  {
   showImage();
   while(1){} 
  }
}
