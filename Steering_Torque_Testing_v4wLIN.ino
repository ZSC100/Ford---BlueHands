/*
 * This code was created from the demo sketch from board:
 * https://www.skpang.co.uk/collections/teensy/products/teensy-4-1-triple-can-bus-board
 *  
 * For board:
 * https://copperhilltech.com/teensy-4-0-can-fd-and-lin-bus-breakout-board/
 * 
 * 
 * 
 */
 
#include <FlexCAN_T4.h>

// Forward declare the callback function so setup() can see it
void handleLKA_Active(const CANFD_message_t &msg);
void handleSCCM_BLeft(const CANFD_message_t &msg);
void sendframe();

const int redPin =  0;
const int greenPin =  2;
const int bluePin =  1;

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

IntervalTimer timer;
uint8_t d=0;
bool stopfd = 0;
unsigned long TimerLED_BIn = 0;
unsigned int counterSCCM_BLeft = 0;
unsigned int TestCounter = 0;
uint8_t LKA_Status = 0;


void canSniff20(const CAN_message_t &msg) { // global callback
  Serial.print("T4: ");
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" BUS "); Serial.print(msg.bus);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.flags.remote);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" IDHIT: "); Serial.print(msg.idhit);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}



void setup(void) {
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(115200); delay(1000);
  Serial.println("Serial Alive");
 
  FD.begin();

  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate =   500000;
  config.baudrateFD = 2000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 75;
  FD.setRegions(64);
  FD.setBaudRate(config);

  // 1. Distribute the mailboxes to their default states
  FD.distribute(); 

  // 2. Accept EVERYTHING on ALL mailboxes globally
  FD.setMBFilter(ACCEPT_ALL); 

  // 3. Restrict Mailboxs to ONLY your specific ID
  FD.setMBFilter(MB1, 0x3CC, STD); // Explicitly Standard ID
  FD.setMBFilter(MB2, 0x83, STD); // Explicitly Standard ID
  
  // 3. Enable all interrupts globally
  FD.enableMBInterrupts(); 

  // 4. Fire this function for EVERY single packet received in each MB
  FD.onReceive(MB1, handleLKA_Active);
  FD.onReceive(MB2, handleSCCM_BLeft);

  //  timer.begin(sendframe, 500000); // Send frame 6,000,000 uS
  digitalWrite(LED_BUILTIN, LOW);
}








void loop() {
  
  FD.events(); /* needed for sequential frame transmit and callback queues */
  //CANFD_message_t msg;

  if (millis() > 250 + TimerLED_BIn)
    {
    digitalWrite(LED_BUILTIN, LOW);
    }
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleLKA_Active(const CANFD_message_t &msg) {
  // Toggle built-in LED so you can physically see if ANY data is arriving
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  LKA_Status = (msg.buf[2] & 0b00000111);

  Serial.print("LKA_Status: ");
  Serial.print(LKA_Status);
  Serial.print(" | TestCounter: ");
  Serial.println(TestCounter);
  TestCounter++;
}

void handleSCCM_BLeft(const CANFD_message_t &msg) {

if (LKA_Status != 2)  // Inhibit follow button trick if no LKA
    {
    counterSCCM_BLeft = 0;
    }

//  Serial.print("Special packet received! ID:");
//  Serial.println(msg.id, HEX);
  if (counterSCCM_BLeft > 100)  // After 10sec
    {
    delayMicroseconds(33000); 
    sendframe();
    delayMicroseconds(33000); 
    sendframe();
    counterSCCM_BLeft = 0;

    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, HIGH);
    TimerLED_BIn = millis();
    }

    counterSCCM_BLeft++;

  // Do your specific processing here
  //Serial.print("Data payload byte 0: ");
  //Serial.println(msg.buf[0]);
}

void handleGlobalSniffer(const CANFD_message_t &msg) {
  // Toggle built-in LED so you can physically see if ANY data is arriving
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  Serial.print("Rx Frame! ID: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(" | Mailbox: ");
  Serial.println(msg.mb);
}

void sendframe()
{
  CANFD_message_t msg;
  msg.len = 8;
  msg.id = 0x83;
  msg.seq = 1;
  msg.buf[0] = 0x00;
  msg.buf[1] = 0xE0;
  msg.buf[2] = 0x80;
  msg.buf[3] = 0x00;
  msg.buf[4] = 0x03;
  msg.buf[5] = 0x08;
  msg.buf[6] = 0x00;
  msg.buf[7] = 0x00;
  FD.write( msg);
}
