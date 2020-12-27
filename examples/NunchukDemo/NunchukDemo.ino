/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.0.0
* @file
*
* NunchukDemo.ino - Demo for the Wii Nunchuk extension controller
* <p>
* Based off project by:
* Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
* Project URL : http://www.gabrielbianconi.com/projects/ArduinoNunchuck/
* Based on the following resources: http://wiibrew.org/wiki/Wiimote/Extension_Controllers
* <p>
* Wii is a trademark of <a href="https://www.nintendo.com/">Nintendo</a>. The author of this library does not claim
* any ownership or association with Nintendo or their affiliates.
* <p>
* MIT License
* <p>
* Copyright (c) 2020 Bryan Stadick
* <p>
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* <p>
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* <p>
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* <p>
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
    Serial.print("AnalogX: ");
    Serial.print(nun.analogX, DEC);
    Serial.print(", AnalogY: ");
    Serial.print(nun.analogY, DEC);
    Serial.print(", AccelX: ");
    Serial.print(nun.accelX, DEC);
    Serial.print(", AccelY: ");
    Serial.print(nun.accelY, DEC);
    Serial.print(", AccelZ: ");
    Serial.print(nun.accelZ, DEC);
    Serial.print(", Z: ");
    Serial.print(nun.zButton, DEC);
    Serial.print(", C: ");
    Serial.println(nun.cButton, DEC);
    
    delay(250);
  }
  else{
    Serial.println("Failed to get data");
  }
}
