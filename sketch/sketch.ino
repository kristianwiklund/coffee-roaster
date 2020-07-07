#include <Encoder.h>

#include <BrewpiBuzzer.h>
#include <BrewpiLCD.h>
#include <BrewpiPins.h>
#include <BrewpiPrint.h>

#include <SPI.h>
#include "Adafruit_MAX31855.h"
//#include <ModbusRtu.h>

// data array for modbus network sharing
uint16_t au16data[16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */

// Pins used by the adafruit thing:
// Try using 1 - (CS), 12 (DO), 13 (SCK)

// Pins used by the brewpi thing
// A4 - onewire
// 3 - buzzer
// 4 - door
// 5 - heating
// 6 - cooling

// 7,8,9 - encoder

// 10 - CS for display


Buzzer buzzer;


//Modbus slave(1,Serial, 0); // is is slave @1 and RS-232 or USB-FTDI

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
#define MAXCS   2
Adafruit_MAX31855 thermocouple(MAXCS);
//MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Encoder myEnc(8, 9);

int relay = 5;  
  
void setup() {
    Serial.begin( 9600 ); // baud-rate at 19200
  //slave.start();
 // slave.begin( 19200); // 19200 baud, 8-bits, even, 1-bit stop
  // use Arduino pins 
  pinMode(relay, OUTPUT);
  display_setup();

  // buzzer
  buzzer.init();
  buzzer.beep(3,100);
   delay(500);

}

void loop() {
   //write current thermocouple value
  au16data[2] = ((uint16_t) thermocouple.readCelsius()*100);

   //poll modbus registers
  // slave.poll( au16data, 16 );

   //write relay value using pwm
  // analogWrite(relay, (au16data[4]/100.0)*255);
   delay(500);
     update_display();
     
}

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
  lcd.setCursor(6,2);
  lcd.write(ftoa(buf,au16data[2]/100.0,1));
   lcd.write((char)223);
   lcd.write(" ");

  lcd.setCursor(6,3);
  lcd.write(ftoa(buf,au16data[4],1));
   lcd.write((char)223);
   lcd.write(" ");

   
}



void display_setup() {
     // display
 lcd.init();
 lcd.begin(20,4);
 lcd.home();
   lcd.clear();
  delay(100); // without this, the first character is not printed
 lcd.write("COFFEE-I-NATOR!");
 lcd.setCursor(0,2);
 lcd.write("Air : ");
 lcd.setCursor(0,3);
 lcd.write("Set : ");

 writetemps();
}

void update_display() {
 writetemps();
}


  
