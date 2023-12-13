#define READ_PIN 3
#define N_SYNC 8
#define N_DATA 8
#define N_SUM 1
#define STARTING_DELAY 100

bool current_state;
String sstate = "None"; 

// the variable to check that the current pulse is starting pulse
bool is_starting_pulse = 1;
bool is_sync_pulse = 0;
bool is_data_pulse = 0;

bool is_measure_start = 0;

unsigned long start_t = 0;
int signal_idx = 0;

bool data[N_DATA + N_SUM];
int data_idx = 0;
int one_data_counter = 0;

int trans_delay = 0;
int trans_i = 0; 

void checkStart(int start_duration)
{
  if (is_measure_start == 0)
  {
    start_t = millis();
    sstate = "Start Starting pulse";
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
      
      sstate = "Starting pulse correct";
      is_measure_start = 0;
      start_t = millis();
    }
    else
    {
      // measure again unitil correct      
      sstate = "Start Starting pulse";
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
  trans_delay = (trans_delay*(trans_i-1) + _t)/(trans_i);
  sstate = String(trans_delay);
  
  if (trans_i >= N_SYNC)
  {
      is_starting_pulse = 0; 
      is_sync_pulse = 0;
      is_data_pulse = 1;
      trans_i = 0;
      sstate = "delay: " + String(trans_delay) + ", Move to data pulse";
  }
  start_t = millis();
}
void checkDataPulse(bool is_check_sum)
{ 
  int duration = int(millis() - start_t);
  if (duration > 0.7*trans_delay)
  {
    data[data_idx] = 0;
    data_idx++;
  }
  else
  {
    one_data_counter++;
    if (one_data_counter >= 2)
    {
      data[data_idx] = 1;
      data_idx++;
      one_data_counter = 0;
    }
  }
  showData();
  if (data_idx >= N_DATA + N_SUM)
  {
      if (is_check_sum && checkSum())
      {
        showData();
      }
      else
      {
        sstate = "incorrect data";
      }
      resetData();
      data_idx = 0;
      
      is_starting_pulse = 1; 
      is_sync_pulse = 0;
      is_data_pulse = 0;
  }
  start_t = millis();
}
bool checkSum()
{
  int sum = 0;
  for(int i=0; i<N_DATA; i++)
  {
    sum += data[i];
  }
  if (sum%2 == data[N_DATA])
    return true;
  else
    return false;
}
void showData()
{
  sstate = "Data = ";
  for(int i=0; i<N_DATA; i++)
  {
    sstate += String(data[i]) + " "; 
  }
  sstate += ", Data_idx: " + String(data_idx);
}
void resetData()
{
  for(int i=0; i<N_DATA + N_SUM; i++)
  {
    data[i] = 0; 
  }
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
    checkDataPulse(1);
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
//  delay(500);
}
