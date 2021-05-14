#include <Encoder.h> // encoder 1.4.1 by Paul Stoffregen

#include <BrewpiBuzzer.h>
#include <BrewpiLCD.h>
#include <BrewpiPins.h>
#include <BrewpiPrint.h>

#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include <Adafruit_MAX31865.h>


#include <PID_v1.h>

double MSP, Input, Output;

PID myPID(&Input, &Output, &MSP,75,50,0, DIRECT);
int WindowSize = 1000;
unsigned long windowStartTime;


// thermocouple: CS 2, SPI normal pins
// pt100: CS A1, SPI normal pins 

// Pins used by the brewpi thing
// A4 - onewire
// 3 - buzzer
// 4 - door 
// 5 - heating (used for relay/heater PWM)
// 6 - cooling (used for fan/MOSFET PWM)

// 7,8,9 - encoder

// 10 - CS for display


Buzzer buzzer;
long oldPosition  = -999;
double temperature=0.0;
double beantemp=0.0;

#define STARTSETPOINT 180
int setpoint = STARTSETPOINT;


// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.

#define MAXCS   2 // 10 is used by the brewpi display board
Adafruit_MAX31855 thermocouple(MAXCS);
Encoder myEnc(8, 9);
#define PT100CS A1
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

Adafruit_MAX31865 thermo = Adafruit_MAX31865(PT100CS);

int relay = 5;  

unsigned long oldtime=0;

SpiLcd lcd;
char buf[10];

// ftoa from skumlerud, http://forum.arduino.cc/index.php?topic=44262.0
char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 if(precision>0) {
   *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
 } else
    *a++=0; 
 return ret;
}


void writetemps() {
  static double otemp=-1.0;
  static int oset=-1.0;
  static int obean=-1.0;
  
  if (otemp != temperature) {
    lcd.setCursor(6,2);
    lcd.write(itoa(((int)(temperature)),buf, 10));
    lcd.write((char)223);
    lcd.write(" ");
      Serial.print(" Air Temperature = "); Serial.println(temperature);
      Serial.println(itoa(((int)(temperature)),buf, 10));
    otemp = temperature;
  }

  if (oset != setpoint) {
    lcd.setCursor(6,3);
    lcd.write(itoa(((int)(setpoint)), buf, 10));
    lcd.write((char)223);
    lcd.write(" ");
    oset=setpoint;
  }

  if (obean != beantemp) {
    lcd.setCursor(6,1);
    lcd.write(itoa(((int)(beantemp)),buf, 10));
    lcd.write((char)223);
    lcd.write(" ");
    obean = beantemp;
            Serial.print("Bean Temperature = "); Serial.println(beantemp);

  }
   
}

// not used with coffee sensor
//void writepow() {
//  static double oout=-1;
//  
//  if (oout != Output) {
//  oout = Output;
//    lcd.setCursor(6,1);
//    lcd.write(itoa((int)(100.0*Output/WindowSize), buf, 10));
//    lcd.write((char)'%');
//    lcd.write(" ");
//  
//  }
//  
//}


void display_setup() {
     // display
 lcd.init();
 lcd.begin(20,4);
 lcd.home();
   lcd.clear();
  delay(100); // without this, the first character is not printed
 lcd.write("COFFEE-I-NATOR 2.1!");
 lcd.setCursor(0,2);
 lcd.write("Air : ");
 lcd.setCursor(0,3);
 lcd.write("Set : ");
 lcd.setCursor(0,1);
 lcd.write("Bean: ");

 writetemps();
 //writepow();
}

void update_display() {
 writetemps();
// writepow();
}

void setup() {
  Serial.begin( 9600 ); // baud-rate at 19200

  pinMode(relay, OUTPUT);
  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  display_setup();
  
  // buzzer
  buzzer.init();
  buzzer.beep(1,100);
  delay(50);

  // pid

  windowStartTime = millis();
  MSP = setpoint;
    buzzer.beep(1,100);
  delay(50);
  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);
  buzzer.beep(1,100);
  delay(50);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  buzzer.beep(1,100);
  delay(50);
}

void read_encoder() {

  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    setpoint = STARTSETPOINT + (newPosition/4)*10;
    if (setpoint > 220) setpoint = 220;
    if (setpoint < 0) setpoint = 0;

    MSP = (double)setpoint;
    
    oldPosition = newPosition;
        Serial.println(newPosition);

  }
  
}

void loop() {
   //write current thermocouple value
   if(millis()-oldtime > 100) {
      oldtime=millis();
      temperature = (thermocouple.readCelsius());
      // for some reason, the display stops working after reading the bean temperature
      beantemp = 1.0*((int)(thermo.temperature(RNOMINAL, RREF)));
      Input = temperature;
      myPID.Compute();
//            Serial.println(MSP);
//      Serial.println(Input);

//      Serial.println(Output);

   }

  if(millis() - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if(Output < millis() - windowStartTime) digitalWrite(relay,HIGH);
  else digitalWrite(relay,LOW);
  
   read_encoder();
   update_display();

}




  
