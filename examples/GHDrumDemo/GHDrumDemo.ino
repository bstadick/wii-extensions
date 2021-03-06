/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.0.0
* @file
*
* GHDrumDemo.ino - Demo for the Wii Guitar Hero Drum extension controller
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

#define KEYPIN 4 // pin for disabling keyboard/enabling serial output (useful for debug)
#define RELPIN 5 // pin for enabling/disabling continuous key press mode
#define BAUDRATE 9600

// key bindings
#define ORKEY 'v'
#define BLKEY 'c'
#define YLKEY 'z'
#define RDKEY 'x'
#define GRKEY 'c'
#define PDKEY ' '
#define PLKEY ' '
#define MNKEY ' '

// previously pressed value variables
boolean or_pressed = false;
boolean bl_pressed = false;
boolean yl_pressed = false;
boolean rd_pressed = false;
boolean gr_pressed = false;
boolean pd_pressed = false;
boolean pl_pressed = false;
boolean mn_pressed = false;

// drum data structure
GHDrumStruct drumStruct;

// drum extension controller object
WiiExt drum = WiiExt();

void setup() {
  // initialize controller as type GH drum
  drum.init(GHDrum_t);
  
  // start keyboard output emulation
  Keyboard.begin();
  
  // start serial
  Serial.begin(BAUDRATE);
  
  // set pinmodes
  pinMode(KEYPIN, INPUT_PULLUP);
  pinMode(RELPIN, INPUT_PULLUP);
}

void loop() {
  
  //delay(10);
  
  // poll drum for update
  if(drum.update()){
    // get updated values
    drumStruct = drum.getGHDrum();
    
    if(digitalRead(KEYPIN)){
      // keyboard enabled
      
      if(digitalRead(RELPIN)){
        // continuous key press (default keyboard action)
        
        // orange button
        if(!or_pressed & drumStruct.orange)
          Keyboard.press(ORKEY);
        else if(or_pressed & !drumStruct.orange)
          Keyboard.release(ORKEY);
        or_pressed = drumStruct.orange;
        
        // blue button
        if(!bl_pressed & drumStruct.blue)
          Keyboard.press(BLKEY);
        else if(bl_pressed & !drumStruct.blue)
          Keyboard.release(BLKEY);
        bl_pressed = drumStruct.blue;
        
        // yellow button
        if(!yl_pressed & drumStruct.yellow)
          Keyboard.press(YLKEY);
        else if(yl_pressed & !drumStruct.yellow)
          Keyboard.release(YLKEY);
        yl_pressed = drumStruct.yellow;
        
        // red button
        if(!rd_pressed & drumStruct.red)
          Keyboard.press(RDKEY);
        else if(rd_pressed & !drumStruct.red)
          Keyboard.release(RDKEY);
        rd_pressed = drumStruct.red;
        
        // green button
        if(!gr_pressed & drumStruct.green)
          Keyboard.press(GRKEY);
        else if(gr_pressed & !drumStruct.green)
          Keyboard.release(GRKEY);
        gr_pressed = drumStruct.green;
        
        // pedal strum
        if(!pd_pressed & drumStruct.pedal)
          Keyboard.press(PDKEY);
        else if(pd_pressed & !drumStruct.pedal)
          Keyboard.release(PDKEY);
        pd_pressed = drumStruct.pedal;
        
        // plus key
        if(!pl_pressed & drumStruct.plus)
          Keyboard.press(PLKEY);
        else if(pl_pressed & !drumStruct.plus)
          Keyboard.release(PLKEY);
        pl_pressed = drumStruct.plus;
        
        // minus key
        if(!mn_pressed & drumStruct.minus)
          Keyboard.press(MNKEY);
        else if(mn_pressed & !drumStruct.minus)
          Keyboard.release(MNKEY);
        mn_pressed = drumStruct.minus;
        
      }
      else{
        // discontinuous key press (release key right after press)
        
        // orange button
        if(!or_pressed & drumStruct.green){
          Keyboard.press(ORKEY);
          Keyboard.release(ORKEY);
        }
        
        // blue button
        if(!bl_pressed & drumStruct.blue){
          Keyboard.press(BLKEY);
          Keyboard.release(BLKEY);
        }
        
        // yellow button
        if(!yl_pressed & drumStruct.yellow){
          Keyboard.press(YLKEY);
          Keyboard.release(YLKEY);
        }
        
        // red button
        if(!rd_pressed & drumStruct.red){
          Keyboard.press(RDKEY);
          Keyboard.release(RDKEY);
        }
        
        // green button
        if(!gr_pressed & drumStruct.green){
          Keyboard.press(GRKEY);
          Keyboard.release(GRKEY);
        }
        
        // pedal strum
        if(!pd_pressed & drumStruct.pedal){
          Keyboard.press(PDKEY);
          Keyboard.release(PDKEY);
        }
        
        // plus key
        if(!pl_pressed & drumStruct.plus){
          Keyboard.press(PLKEY);
          Keyboard.release(PLKEY);
        }
        
        // minus key
        if(!mn_pressed & drumStruct.minus){
          Keyboard.press(MNKEY);
          Keyboard.release(MNKEY);
        }
        
      }
    }
    else{
      // serial enabled
      Serial.print("AnalogX: ");
      Serial.print(drumStruct.analogX);
      Serial.print(", AnalogY: ");
      Serial.print(drumStruct.analogY);
      Serial.print(", VelPresent: ");
      if(drumStruct.isVelocity)
        Serial.print("true");
      else
        Serial.print("false");
      Serial.print(", IsHighHat: ");
      if(drumStruct.isHighHat)
        Serial.print("true");
      else
        Serial.print("false");
      Serial.print(", Velocity: ");
      Serial.print(drumStruct.velocity);
      Serial.print(", VelSrc: ");
      Serial.print(drum.velTypeToStr(drumStruct.velocityType));
      Serial.print(", green: ");
      Serial.print(drumStruct.green);
      Serial.print(", red: ");
      Serial.print(drumStruct.red);
      Serial.print(", yellow: ");
      Serial.print(drumStruct.yellow);
      Serial.print(", blue: ");
      Serial.print(drumStruct.blue);
      Serial.print(", orange: ");
      Serial.print(drumStruct.orange);
      Serial.print(", pedal: ");
      Serial.print(drumStruct.pedal);
      Serial.print(", Plus: ");
      Serial.print(drumStruct.plus);
      Serial.print(", Minus: ");
      Serial.println(drumStruct.minus);
      /*u_int* values = drum.getValues();
      for(int i = 0; i < 6; i++){
        Serial.print(values[i]);
        Serial.print(", ");
      }
      Serial.println();*/
      
      delay(250);
    }
  }
  else{
    if(!digitalRead(KEYPIN)){
      Serial.println("Failed to get data");
    }
  }
}
