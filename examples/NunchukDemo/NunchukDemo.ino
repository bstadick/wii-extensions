/*
 * WiiExtDemo.ino
 *
 * Based off project by:
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 * Project URL : http://www.gabrielbianconi.com/projects/ArduinoNunchuck/
 *
 */

#include <Wire.h>
#include <WiiExt.h>

#define BAUDRATE 9600

NunchukStruct nun;

WiiExt nunchuk = WiiExt();

void setup()
{
  // initizliae controller as type Nunchuk controller
  nunchuk.init(Nunchuk_t);
  
  // start serial
  Serial.begin(BAUDRATE);
}

void loop()
{
  // poll Nunchuk for update
  if(nunchuk.update()){
    // get updated values
    nun = nunchuk.getNunchuk();
    
    // print out data
    Serial.print("JX: ");
    Serial.print(nun.analogX, DEC);
    Serial.print(" JY: ");
    Serial.print(nun.analogY, DEC);
    Serial.print(" AX: ");
    Serial.print(nun.accelX, DEC);
    Serial.print(" AY: ");
    Serial.print(nun.accelY, DEC);
    Serial.print(" AZ: ");
    Serial.print(nun.accelZ, DEC);
    Serial.print(" BZ: ");
    Serial.print(nun.zButton, DEC);
    Serial.print(" BC: ");
    Serial.println(nun.cButton, DEC);
  }
  else{
    Serial.println("Failed to get data");
  }
}
