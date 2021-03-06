# 1 "/home/jdv/Project/arduino/frequncy_new/frequncy_new.ino"
# 1 "/home/jdv/Project/arduino/frequncy_new/frequncy_new.ino"
//#include <I2C_Anything.h>
// /home/jdv/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/bin/avrdude -C/home/jdv/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:/tmp/arduino_build_508038/frequncy_new.ino.hex:i 
//#include <RS485_non_blocking.h>
# 5 "/home/jdv/Project/arduino/frequncy_new/frequncy_new.ino" 2
# 6 "/home/jdv/Project/arduino/frequncy_new/frequncy_new.ino" 2
//#include <Wire.h>



const unsigned long BAUD_RATE = 19200;
const float TIME_PER_BYTE = 1.0 / (BAUD_RATE / 10.0); // seconds per sending one byte
//const unsigned long PACKET_LENGTH = ((sizeof (message) * 2) + 6); // 2 bytes per payload byte plus STX/ETC/CRC
//const unsigned long PACKET_TIME =  TIME_PER_BYTE * PACKET_LENGTH * 1000000;  // microseconds

// software serial pins
const byte RX_PIN = 8;
const byte TX_PIN = 9;

const byte ENABLE_PIN = 4;
const byte LED_PIN = 13;

const int sensorIn = A7;
int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module

static bool RequestCalled;
int number = 0;

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;

int BUTTON_IN = 3;
int BUTTON_OUT = 2;

int working = 1;
int received ;

SoftwareSerial mySerial (RX_PIN, TX_PIN);


// callback routines


void setup(){
  // debugging prints
  Serial.begin (115200);

    pinMode (ENABLE_PIN, 0x1); // driver output enable
    pinMode(BUTTON_IN, 0x0);
    pinMode(BUTTON_OUT, 0x1);
    pinMode(13, 0x1);

    digitalWrite(ENABLE_PIN, 0x0);
    digitalWrite(BUTTON_OUT, 0x0);

    mySerial.begin(4800);

    Serial.println("Starting");

}

void loop(){

  working = digitalRead ( BUTTON_IN );

  if (mySerial.available()) {
    received = mySerial.read();
    delay(10);
    Serial.write(received);
    delay(10);
    if( received == '1' ) {
      delay(10);
      mySerial.write("opening_remotaly");
      digitalWrite(BUTTON_OUT, 0x1);
      delay(5000);
      digitalWrite(BUTTON_OUT, 0x0);
    }
  }

  //For Manual Work
 if (working) {
  digitalWrite(13, 0x1);

  Voltage = getVPP();
  VRMS = (Voltage/2.0) *0.707;
  AmpsRMS = (VRMS * 1000)/mVperAmp;

  Serial.print(AmpsRMS);
  Serial.println(" Amps RMS");

  if( AmpsRMS < 1 ) {
    mySerial.write("opening_manualy");
    delay(2000);
    digitalWrite(BUTTON_OUT, 0x1);
    delay(5000);
//    I2C_writeAnything ("Open");
  }
  digitalWrite(BUTTON_OUT, 0x0);
 } else {
  digitalWrite(13, 0x0);
 }

}

float getVPP()
{
  float result;

  int readValue; //value read from the sensor
  int maxValue = 0; // store max value here
  int minValue = 1024; // store min value here

   uint32_t start_time = millis();
   while((millis()-start_time) < 100) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue)
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue)
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }

   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;

   return result;
 }
