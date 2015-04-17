#include <Wire.h>
#include <WiiExt.h>

#define KEYPIN 4 // pin for disabling keyboard/enabling serial output (useful for debug)
#define RELPIN 5 // pin for enabling/disabling continuous key press mode
#define BAUDRATE 9600

// key bindings
#define RLKEY 'z'
#define CLKEY 'x'
#define CRKEY 'c'
#define RRKEY 'v'

// previously pressed value variables
boolean cl_pressed = false;
boolean cr_pressed = false;
boolean rl_pressed = false;
boolean rr_pressed = false;

// taiko drum data structure
TaikoStruct taikoStruct;

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
    taikoStruct = drum.getTaiko();
    
    if(digitalRead(KEYPIN)){
      // keyboard enabled
      
      if(digitalRead(RELPIN)){
        // continuous key press (default keyboard action)
        
        // left rim value
        if(!rl_pressed & taikoStruct.rimLeft)
          Keyboard.press(RLKEY);
        else if(rl_pressed & !taikoStruct.rimLeft)
          Keyboard.release(RLKEY);
        rl_pressed = taikoStruct.rimLeft;
        
        // left center value
        if(!cl_pressed & taikoStruct.centerLeft)
          Keyboard.press(CLKEY);
        else if(cl_pressed & !taikoStruct.centerLeft)
          Keyboard.release(CLKEY);
        cl_pressed = taikoStruct.centerLeft;
        
        // right center value
        if(!cr_pressed & taikoStruct.centerRight)
          Keyboard.press(CRKEY);
        else if(cr_pressed & !taikoStruct.centerRight)
          Keyboard.release(CRKEY);
        cr_pressed = taikoStruct.centerRight;
        
        // right rim value
        if(!rr_pressed & taikoStruct.rimRight)
          Keyboard.press(RRKEY);
        else if(rr_pressed & !taikoStruct.rimRight)
          Keyboard.release(RRKEY);
        rr_pressed = taikoStruct.rimRight;
      }
      else{
        // discontinuous key press (release key right after press)
        
        // left rim value
        if(taikoStruct.rimLeft){
          Keyboard.press(RLKEY);
          Keyboard.release(RLKEY);
        }
        
        // left center value
        if(taikoStruct.centerLeft){
          Keyboard.press(CLKEY);
          Keyboard.release(CLKEY);
        }
        
        // right center value
        if(taikoStruct.centerRight){
          Keyboard.press(CRKEY);
          Keyboard.release(CRKEY);
        }
        
        // right rim value
        if(taikoStruct.rimRight){
          Keyboard.press(RRKEY);
          Keyboard.release(RRKEY);
        }
      }
    }
    else{
      // serial enabled
      Serial.print("RL: ");
      Serial.print(taikoStruct.rimLeft);
      Serial.print(", CL: ");
      Serial.print(taikoStruct.centerLeft);
      Serial.print(", CR: ");
      Serial.print(taikoStruct.centerRight);
      Serial.print(", RR: ");
      Serial.println(taikoStruct.rimRight);
      
      delay(250);
    }
  }
  else{
    if(!digitalRead(KEYPIN)){
      Serial.println("Failed to get data");
    }
  }
}
