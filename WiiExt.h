/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.0.0
* @file
*
* WiiExt.h - Improved Wii Nunchuk library for Arduino
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

#ifndef WiiExt_H
#define WiiExt_H

#include <Arduino.h>

#define ADDRESS 0x52
#define MAXMSGSIZE 0x20

typedef enum WiiExtType {Nunchuk_t = 1, Taiko_t, GHGuitar_t, GHDrum_t} WiiExtType;

typedef enum GHTouch {GHNT = 0, GHF1, GHF12, GHF2, GHF23, GHF3, GHF34, GHF4, GHF45, GHF5} GHTouch;
typedef enum GHVelType {GHNV = 0, GHR, GHY, GHB, GHO, GHG, GHP, GHHH} GHVelType;
typedef enum GHVelocity {GHVVH = 0, GHVMH, GHVH, GHVM, GHVS, GHVMS, GHVVS, GHVNH} GHVelocity;

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

typedef struct GHGuitarStruct {
    int analogX;
    int analogY;
    int touchBarRaw;
    GHTouch touchBar;
    int whammyBar;
    bool green;
    bool red;
    bool yellow;
    bool blue;
    bool orange;
    bool strumUp;
    bool strumDown;
    bool plus;
    bool minus;
    bool isGH3;
    
} GHGuitarStruct;

typedef struct GHDrumStruct {
    int analogX;
    int analogY;
    bool green;
    bool red;
    bool yellow;
    bool blue;
    bool orange;
    bool pedal;
    bool plus;
    bool minus;
    bool isVelocity;
    bool isHighHat;
    GHVelocity velocity;
    GHVelType velocityType;
} GHDrumStruct;

class WiiExt
{
    public:
        void init();
        void init(WiiExtType t);
        bool update();
        char* touchToStr(GHTouch value);
        char* velTypeToStr(GHVelType value);
        char* velValueToStr(GHVelocity value);
        WiiExtType getType(){return _type;}
        int* getValues(){return _values;}
        NunchukStruct getNunchuk(){return _nunchuk;}
        TaikoStruct getTaiko(){return _taiko;}
        GHGuitarStruct getGHGuitar(){return _ghguitar;}
        GHDrumStruct getGHDrum(){return _ghdrum;}

    private:
        NunchukStruct _nunchuk;
        TaikoStruct _taiko;
        GHGuitarStruct _ghguitar;
        GHDrumStruct _ghdrum;
        WiiExtType _type;
        int _values[MAXMSGSIZE];
        void _sendByte(byte data, byte location);
        void _identify();
        bool _updateNunchuk();
        bool _updateTaiko();
        bool _updateGHGuitar();
        bool _updateGHDrum();
};

#endif
