#define DEBUG   1
const char DELIMITER = ',';
unsigned long previousMillis = 0; 
const long interval = 1000;

#include "DHMonitor.h"
DHMonitor monitor;
/**
 * Writing on SD card
 *
 * SD card uses SPI bus:
 * MOSI       - pin 11
 * MISO       - pin 12
 * CLK or SCK - pin 13
 * CS         - pin 10
 *
 * SPI for Serial Peripheral Interface
 *
 * created  24 Nov 2010
 * modified 9 Apr 2012
 * by Tom Igoe
 * cf: https://www.arduino.cc/en/Tutorial/Datalogger
 */
#include <SPI.h>


// Arduino Pro mini pin 10
// cf: https://www.arduino.cc/en/Reference/SPI
const int chipSelect = 10;

volatile bool sDisReady = false; // only try to write if sd is ready (detected)

/**
 * build a string with time
 */
String buildTime()
{
  String dateString = monitor.getTime();

  return dateString;
}

String buildString()
{

  String dataString = buildTime();

  dataString += DELIMITER;

  float t1 = 12.2;


  dataString += String(t1);
  dataString += DELIMITER;

  dataString += DELIMITER;
  // vcc  logging
  //float vcc = monitor.readVcc();
  dataString += String(4.8);
  dataString += DELIMITER;

  return dataString;
}


void setup() {
  monitor.ledOn();
  // Serial port initialisation (to communicate with computer)
  //Serial.begin(115200);
  #if DEBUG
    Serial.begin(115200);
    Serial.println("start");
  #endif
  sDisReady = monitor.initSdCard(chipSelect);
}

void loop() {
    unsigned long currentMillis = millis();
    if ( currentMillis - previousMillis >= interval ) {
      previousMillis = currentMillis;

      String tolog = buildString();
      #if DEBUG
        Serial.println(tolog);   // send "file line" to computer serial
        Serial.flush();delay(5); // needed to flush serial when woke up
      #endif

      if (sDisReady) {
        monitor.writeLine(tolog);
      } else {
        #if DEBUG
          Serial.println("could not write log");   // send "file line" to computer serial
          Serial.flush();delay(5); // needed to flush serial when woke up
        #endif
      }
    }

}


