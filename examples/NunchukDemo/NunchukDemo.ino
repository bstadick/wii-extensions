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
