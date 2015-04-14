/*
 * WiiExt.h - Improved improved Wii Nunchuk library for Arduino
 *
 * Based off project by:
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 * Project URL : http://www.gabrielbianconi.com/projects/ArduinoNunchuck/
 *
 * Based on the following resources:
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
 */

#ifndef WiiExt_H
#define WiiExt_H

#include <Arduino.h>

#define ADDRESS 0x52
#define MAXMSGSIZE 0x20

typedef enum WiiExtType {Nunchuk_t = 1, Taiko_t} WiiExtType;

typedef struct NunchukStruct {
	int analogX;
    int analogY;
    int accelX;
    int accelY;
    int accelZ;
    int zButton;
    int cButton;
} NunchukStruct;

typedef struct TaikoStruct {
	bool centerLeft;
	bool centerRight;
	bool rimLeft;
	bool rimRight;
} TaikoStruct;

class WiiExt
{
  public:
    void init();
	void init(WiiExtType t);
    bool update();
    WiiExtType getType(){return _type;}
    int* getValues(){return _values;}
    NunchukStruct getNunchuk(){return _nunchuk;}
    TaikoStruct getTaiko(){return _taiko;}

  private:
  	NunchukStruct _nunchuk;
    TaikoStruct _taiko;
    WiiExtType _type;
    int _values[MAXMSGSIZE];
    void _sendByte(byte data, byte location);
	void _identify();
	bool _updateNunchuk();
	bool _updateTaiko();
    
};

#endif
