#include <Wire.h>
#include <WiiExt.h>

#define KEYPIN 4 // pin for disabling keyboard/enabling serial output (useful for debug)
#define RELPIN 5 // pin for enabling/disabling continuous key press mode
#define BAUDRATE 9600

// key bindings
#define ORKEY ' '
#define BLKEY 'k'
#define YLKEY 'j'
#define RDKEY 'f'
#define GRKEY 'd'
#define UPKEY ' '
#define DWKEY ' '
#define PLKEY ' '
#define MNKEY ' '

// previously pressed value variables
boolean or_pressed = false;
boolean bl_pressed = false;
boolean yl_pressed = false;
boolean rd_pressed = false;
boolean gr_pressed = false;
boolean up_pressed = false;
boolean dw_pressed = false;
boolean pl_pressed = false;
boolean mn_pressed = false;

// guitar data structure
GHGuitarStruct guitarStruct;

// guitar extension controller object
WiiExt guitar = WiiExt();

void setup() {
  // initialize controller as type GH guitar
  guitar.init(GHGuitar_t);
  
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
  
  // poll guitar for update
  if(guitar.update()){
    // get updated values
    guitarStruct = guitar.getGHGuitar();
    
    if(digitalRead(KEYPIN)){
      // keyboard enabled
      
      if(digitalRead(RELPIN)){
        // continuous key press (default keyboard action)
        
        // orange button
        if(!or_pressed & guitarStruct.green)
          Keyboard.press(ORKEY);
        else if(or_pressed & !guitarStruct.green)
          Keyboard.release(ORKEY);
        or_pressed = guitarStruct.green;
        
        // blue button
        if(!bl_pressed & guitarStruct.blue)
          Keyboard.press(BLKEY);
        else if(bl_pressed & !guitarStruct.blue)
          Keyboard.release(BLKEY);
        bl_pressed = guitarStruct.blue;
        
        // yellow button
        if(!yl_pressed & guitarStruct.yellow)
          Keyboard.press(YLKEY);
        else if(yl_pressed & !guitarStruct.yellow)
          Keyboard.release(YLKEY);
        yl_pressed = guitarStruct.yellow;
        
        // red button
        if(!rd_pressed & guitarStruct.red)
          Keyboard.press(RDKEY);
        else if(rd_pressed & !guitarStruct.red)
          Keyboard.release(RDKEY);
        rd_pressed = guitarStruct.red;
        
        // green button
        if(!gr_pressed & guitarStruct.green)
          Keyboard.press(GRKEY);
        else if(gr_pressed & !guitarStruct.green)
          Keyboard.release(GRKEY);
        gr_pressed = guitarStruct.green;
        
        // up strum
        if(!up_pressed & guitarStruct.strumUp)
          Keyboard.press(UPKEY);
        else if(up_pressed & !guitarStruct.strumUp)
          Keyboard.release(UPKEY);
        up_pressed = guitarStruct.strumUp;
        
        // down strum
        if(!dw_pressed & guitarStruct.strumDown)
          Keyboard.press(DWKEY);
        else if(dw_pressed & !guitarStruct.strumDown)
          Keyboard.release(DWKEY);
        dw_pressed = guitarStruct.strumDown;
        
        // plus key
        if(!pl_pressed & guitarStruct.plus)
          Keyboard.press(PLKEY);
        else if(pl_pressed & !guitarStruct.plus)
          Keyboard.release(PLKEY);
        pl_pressed = guitarStruct.plus;
        
        // minus key
        if(!mn_pressed & guitarStruct.minus)
          Keyboard.press(MNKEY);
        else if(mn_pressed & !guitarStruct.minus)
          Keyboard.release(MNKEY);
        mn_pressed = guitarStruct.minus;
        
      }
      else{
        // discontinuous key press (release key right after press)
        
        // orange button
        if(!or_pressed & guitarStruct.green){
          Keyboard.press(ORKEY);
          Keyboard.release(ORKEY);
        }
        
        // blue button
        if(!bl_pressed & guitarStruct.blue){
          Keyboard.press(BLKEY);
          Keyboard.release(BLKEY);
        }
        
        // yellow button
        if(!yl_pressed & guitarStruct.yellow){
          Keyboard.press(YLKEY);
          Keyboard.release(YLKEY);
        }
        
        // red button
        if(!rd_pressed & guitarStruct.red){
          Keyboard.press(RDKEY);
          Keyboard.release(RDKEY);
        }
        
        // green button
        if(!gr_pressed & guitarStruct.green){
          Keyboard.press(GRKEY);
          Keyboard.release(GRKEY);
        }
        
        // up strum
        if(!up_pressed & guitarStruct.strumUp){
          Keyboard.press(UPKEY);
          Keyboard.release(UPKEY);
        }
        
        // down strum
        if(!dw_pressed & guitarStruct.strumDown){
          Keyboard.press(DWKEY);
          Keyboard.release(DWKEY);
        }
        
        // plus key
        if(!pl_pressed & guitarStruct.plus){
          Keyboard.press(PLKEY);
          Keyboard.release(PLKEY);
        }
        
        // minus key
        if(!mn_pressed & guitarStruct.minus){
          Keyboard.press(MNKEY);
          Keyboard.release(MNKEY);
        }
        
      }
    }
    else{
      // serial enabled
      Serial.print("IsGH3: ");
      if(guitarStruct.isGH3)
        Serial.print("true");
      else
        Serial.print("false");
      Serial.print(", AnalogX: ");
      Serial.print(guitarStruct.analogX);
      Serial.print(", AnalogY: ");
      Serial.print(guitarStruct.analogY);
      Serial.print(", TouchBar: ");
      Serial.print(guitar.touchToStr(guitarStruct.touchBar));
      Serial.print(", WhammyBar: ");
      Serial.print(guitarStruct.whammyBar);
      Serial.print(", green: ");
      Serial.print(guitarStruct.green);
      Serial.print(", red: ");
      Serial.print(guitarStruct.red);
      Serial.print(", yellow: ");
      Serial.print(guitarStruct.yellow);
      Serial.print(", blue: ");
      Serial.print(guitarStruct.blue);
      Serial.print(", orange: ");
      Serial.print(guitarStruct.orange);
      Serial.print(", StrumUp: ");
      Serial.print(guitarStruct.strumUp);
      Serial.print(", StrumDown: ");
      Serial.print(guitarStruct.strumDown);
      Serial.print(", Plus: ");
      Serial.print(guitarStruct.plus);
      Serial.print(", Minus: ");
      Serial.println(guitarStruct.minus);
      
      delay(250);
    }
  }
  else{
    if(!digitalRead(KEYPIN)){
      Serial.println("Failed to get data");
    }
  }
}
