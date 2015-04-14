#include <Wire.h>
#include <WiiExt.h>

#define KEYPIN 4 // pin for disabling keyboard/enabling serial output (useful for debug)
#define RELPIN 5 // pin for enabling/disabling continuous key press mode
#define BAUDRATE 9600

// previously pressed value variables
boolean cl_pressed = false;
boolean cr_pressed = false;
boolean rl_pressed = false;
boolean rr_pressed = false;

// taiko drum data structure
TaikoStruct taiko;

// drum extension controller object
WiiExt drum = WiiExt();

void setup() {
  // initialize controller as type Taiko drum
  drum.init(Taiko_t);
  
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
    taiko = drum.getTaiko();
    
    if(digitalRead(KEYPIN)){
      // keyboard enabled
      
      if(digitalRead(RELPIN)){
        // continuous key press (default keyboard action)
        
        // left rim value (z key)
        if(!rl_pressed & taiko.rimLeft)
          Keyboard.press('z');
        else if(rl_pressed & !taiko.rimLeft)
          Keyboard.release('z');
        rl_pressed = taiko.rimLeft;
        
        // left center value (x key)
        if(!cl_pressed & taiko.centerLeft)
          Keyboard.press('x');
        else if(cl_pressed & !taiko.centerLeft)
          Keyboard.release('x');
        cl_pressed = taiko.centerLeft;
        
        // right center value (c key)
        if(!cr_pressed & taiko.centerRight)
          Keyboard.press('c');
        else if(cr_pressed & !taiko.centerRight)
          Keyboard.release('c');
        cr_pressed = taiko.centerRight;
        
        // right rim value (v key)
        if(!rr_pressed & taiko.rimRight)
          Keyboard.press('v');
        else if(rr_pressed & !taiko.rimRight)
          Keyboard.release('v');
        rr_pressed = taiko.rimRight;
      }
      else{
        // discontinuous key press (release key right after press)
        
        // left rim value (z key)
        if(taiko.rimLeft){
          Keyboard.press('z');
          Keyboard.release('z');
        }
        
        // left center value (x key)
        if(taiko.centerLeft){
          Keyboard.press('x');
          Keyboard.release('x');
        }
        
        // right center value (c key)
        if(taiko.centerRight){
          Keyboard.press('c');
          Keyboard.release('c');
        }
        
        // right rim value (v key)
        if(taiko.rimRight){
          Keyboard.press('v');
          Keyboard.release('v');
        }
      }
    }
    else{
      // serial enabled
      Serial.print("Raw: ");
      Serial.print(drum.getValues()[4]);
      Serial.print(" ,RL: ");
      Serial.print(taiko.rimLeft);
      Serial.print(", CL: ");
      Serial.print(taiko.centerLeft);
      Serial.print(", CR: ");
      Serial.print(taiko.centerRight);
      Serial.print(", RR: ");
      Serial.println(taiko.rimRight);
    }
  }
  else{
    if(!digitalRead(KEYPIN)){
      Serial.println("Failed to get data");
    }
  }
}
