/**
* @author Bryan Stadick - stadi012@umn.edu
* @version 1.0.0
* @file
*
* WiiExt.cpp - Improved Wii Nunchuk library for Arduino
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

#include <Arduino.h>
#include <Wire.h>
#include "WiiExt.h"

// initialize the controller, query for the type of controller
void WiiExt::init()
{
    Wire.begin();

    // initialize the Wii extension controller (works for all controllers)
    WiiExt::_sendByte(0x55, 0xF0);
    WiiExt::_sendByte(0x00, 0xFB);

    // identify the connected controller
    WiiExt::_identify();

    // get first message from the controller
    WiiExt::update();
}

// initialize the controller to the given type of controller
void WiiExt::init(WiiExtType t)
{
    Wire.begin();

    // initialize the Wii extension controller (works for all controllers)
    WiiExt::_sendByte(0x55, 0xF0);
    WiiExt::_sendByte(0x00, 0xFB);

    // identify the connected controller
    WiiExt::_type = t;

    // get first message from the controller
    WiiExt::update();
}

    // identify the connected controller
void WiiExt::_identify()
{
    int count = 0;

    // request data
    Wire.requestFrom(ADDRESS, MAXMSGSIZE);

    // read data
    while(Wire.available())
    {
        WiiExt::_values[count] = Wire.read();
        count++;
    }

    // TODO - parse identifying bytes and set WiiExt::type
  
}

// get updated data from controller
bool WiiExt::update()
{
    switch(WiiExt::_type)
    {
        case Nunchuk_t:
            return WiiExt::_updateNunchuk();
        case Taiko_t:
            return WiiExt::_updateTaiko();
        case GHGuitar_t:
            return WiiExt::_updateGHGuitar();
        case GHDrum_t:
            return WiiExt::_updateGHDrum();
        default:
            return false;
    }
}

// update the Nunchuk data
bool WiiExt::_updateNunchuk()
{
    int count = 0;

    // request data
    if(Wire.requestFrom(ADDRESS, 6) != 6)
      return false;

    // read data
    while(Wire.available())
    {
        WiiExt::_values[count] = Wire.read();
        count++;
    }

    // parse data
    WiiExt::_nunchuk.analogX = WiiExt::_values[0];
    WiiExt::_nunchuk.analogY = WiiExt::_values[1];
    WiiExt::_nunchuk.accelX = (WiiExt::_values[2] << 2) | ((WiiExt::_values[5] >> 2) & 3);
    WiiExt::_nunchuk.accelY = (WiiExt::_values[3] << 2) | ((WiiExt::_values[5] >> 4) & 3);
    WiiExt::_nunchuk.accelZ = (WiiExt::_values[4] << 2) | ((WiiExt::_values[5] >> 6) & 3);
    WiiExt::_nunchuk.zButton = !((WiiExt::_values[5] >> 0) & 1);
    WiiExt::_nunchuk.cButton = !((WiiExt::_values[5] >> 1) & 1);

    // end request?
    WiiExt::_sendByte(0x00, 0x00);
    return true;
}

// update the Taiko data
bool WiiExt::_updateTaiko()
{
    int count = 0;

    // request data
    if(Wire.requestFrom(ADDRESS, 5) != 5)
        return false;

    // read data
    while(Wire.available())
    {
        WiiExt::_values[count] = Wire.read();
        count++;
    }

    // parse data (found on 5th byte and is inverted)
    WiiExt::_values[4] = ~WiiExt::_values[4];
    WiiExt::_taiko.centerLeft = (WiiExt::_values[4] & 0x40);
    WiiExt::_taiko.rimLeft = (WiiExt::_values[4] & 0x20);
    WiiExt::_taiko.centerRight = (WiiExt::_values[4] & 0x10);
    WiiExt::_taiko.rimRight = (WiiExt::_values[4] & 0x08);

    // end request?
    WiiExt::_sendByte(0x00, 0x00);
    return true;
}

bool WiiExt::_updateGHGuitar()
{
    int count = 0;
    
    // request data
    if(Wire.requestFrom(ADDRESS, 6) != 6)
        return false;

    // read data
    while(Wire.available())
    {
        WiiExt::_values[count] = Wire.read();
        count++;
    }

    // parse data
    // BUG - WT guitar is missing y-axis data for analog stick (temp fix is to ignore lack of data)
    WiiExt::_ghguitar.isGH3 = (WiiExt::_values[0] & 0x80);
    if(WiiExt::_ghguitar.isGH3)
    {
        WiiExt::_values[4] = ~WiiExt::_values[4];
        WiiExt::_values[5] = ~WiiExt::_values[5];
        WiiExt::_ghguitar.orange = (WiiExt::_values[5] & 0x80);
        WiiExt::_ghguitar.red = (WiiExt::_values[5] & 0x40);
        WiiExt::_ghguitar.blue = (WiiExt::_values[5] & 0x20);
        WiiExt::_ghguitar.green = (WiiExt::_values[5] & 0x10);
        WiiExt::_ghguitar.yellow = (WiiExt::_values[5] & 0x08);
        WiiExt::_ghguitar.strumUp = (WiiExt::_values[5] & 0x01);
        WiiExt::_ghguitar.strumDown = (WiiExt::_values[4] & 0x40);
        WiiExt::_ghguitar.minus = (WiiExt::_values[4] & 0x10);
        WiiExt::_ghguitar.plus = (WiiExt::_values[4] & 0x04);
        WiiExt::_ghguitar.whammyBar = (WiiExt::_values[3] & 0x1F);
        WiiExt::_ghguitar.touchBarRaw = (WiiExt::_values[2] & 0x1F);
        WiiExt::_ghguitar.analogY = (WiiExt::_values[1] & 0x3F);
        WiiExt::_ghguitar.analogX = (WiiExt::_values[0] & 0x3F);
    }
    else{
        WiiExt::_values[3] = ~WiiExt::_values[3];
        WiiExt::_values[4] = ~WiiExt::_values[4];
        WiiExt::_ghguitar.orange = (WiiExt::_values[4] & 0x80);
        WiiExt::_ghguitar.red = (WiiExt::_values[4] & 0x40);
        WiiExt::_ghguitar.blue = (WiiExt::_values[4] & 0x20);
        WiiExt::_ghguitar.green = (WiiExt::_values[4] & 0x10);
        WiiExt::_ghguitar.yellow = (WiiExt::_values[4] & 0x08);
        WiiExt::_ghguitar.strumUp = (WiiExt::_values[4] & 0x01);
        WiiExt::_ghguitar.strumDown = (WiiExt::_values[3] & 0x40);
        WiiExt::_ghguitar.minus = (WiiExt::_values[3] & 0x10);
        WiiExt::_ghguitar.plus = (WiiExt::_values[3] & 0x04);
        WiiExt::_ghguitar.whammyBar = (WiiExt::_values[2] & 0x1F);
        WiiExt::_ghguitar.touchBarRaw = (WiiExt::_values[1] & 0x1F);
        //WiiExt::_ghguitar.analogY = (WiiExt::_values[0] & 0x3F);
        WiiExt::_ghguitar.analogX = (WiiExt::_values[0] & 0x3F);
    }

    if(WiiExt::_ghguitar.isGH3){
        WiiExt::_ghguitar.touchBar = GHNT;
    }
    else{
        switch(WiiExt::_ghguitar.touchBarRaw)
        {
            case 0x0F:
                WiiExt::_ghguitar.touchBar = GHNT;
                break;
            case 0x04:
                WiiExt::_ghguitar.touchBar = GHF1;
                break;
            case 0x07:
                WiiExt::_ghguitar.touchBar = GHF12;
                break;
            case 0x0A:
                WiiExt::_ghguitar.touchBar = GHF2;
                break;
            case 0x0C:
            case 0x0D:
                WiiExt::_ghguitar.touchBar = GHF23;
                break;
            case 0x12:
            case 0x13:
                WiiExt::_ghguitar.touchBar = GHF3;
                break;
            case 0x14:
            case 0x15:
                WiiExt::_ghguitar.touchBar = GHF34;
                break;
            case 0x17:
            case 0x18:
                WiiExt::_ghguitar.touchBar = GHF4;
                break;
            case 0x1A:
                WiiExt::_ghguitar.touchBar = GHF45;
                break;
            case 0x1F:
                WiiExt::_ghguitar.touchBar = GHF5;
                break;
            default:
                WiiExt::_ghguitar.touchBar = GHNT;
                break;
        }
    }
    
    // end request?
    WiiExt::_sendByte(0x00, 0x00);
    return true;
}

bool WiiExt::_updateGHDrum()
{
    int count = 0;
    int which;
    
    // request data
    if(Wire.requestFrom(ADDRESS, 6) != 6)
        return false;

    // read data
    while(Wire.available())
    {
        WiiExt::_values[count] = Wire.read();
        count++;
    }

    // parse data
    // BUG - WT drum is missing y-axis data for analog stick (temp fix is to ignore lack of data)
    if(false)
    {
        WiiExt::_values[4] = ~WiiExt::_values[4];
        WiiExt::_values[5] = ~WiiExt::_values[5];
        WiiExt::_ghdrum.orange = (WiiExt::_values[5] & 0x80);
        WiiExt::_ghdrum.red = (WiiExt::_values[5] & 0x40);
        WiiExt::_ghdrum.yellow = (WiiExt::_values[5] & 0x20);
        WiiExt::_ghdrum.green = (WiiExt::_values[5] & 0x10);
        WiiExt::_ghdrum.blue = (WiiExt::_values[5] & 0x08);
        WiiExt::_ghdrum.pedal = (WiiExt::_values[5] & 0x04);
        WiiExt::_ghdrum.minus = (WiiExt::_values[4] & 0x10);
        WiiExt::_ghdrum.plus = (WiiExt::_values[4] & 0x04);
        WiiExt::_ghdrum.velocity = (GHVelocity)(WiiExt::_values[3] & 0xE0);
        WiiExt::_ghdrum.isHighHat = !(WiiExt::_values[2] & 0x80);
        WiiExt::_ghdrum.isVelocity = !(WiiExt::_values[2] & 0x40);
        which = (WiiExt::_values[2] & 0x3E);
        WiiExt::_ghdrum.analogY = (WiiExt::_values[1] & 0x3F);
        WiiExt::_ghdrum.analogX = (WiiExt::_values[0] & 0x3F);
    }
    else{
        WiiExt::_values[3] = ~WiiExt::_values[3];
        WiiExt::_values[4] = ~WiiExt::_values[4];
        WiiExt::_ghdrum.orange = (WiiExt::_values[4] & 0x80);
        WiiExt::_ghdrum.red = (WiiExt::_values[4] & 0x40);
        WiiExt::_ghdrum.yellow = (WiiExt::_values[4] & 0x20);
        WiiExt::_ghdrum.green = (WiiExt::_values[4] & 0x10);
        WiiExt::_ghdrum.blue = (WiiExt::_values[4] & 0x08);
        WiiExt::_ghdrum.pedal = (WiiExt::_values[4] & 0x04);
        WiiExt::_ghdrum.minus = (WiiExt::_values[3] & 0x10);
        WiiExt::_ghdrum.plus = (WiiExt::_values[3] & 0x04);
        WiiExt::_ghdrum.velocity = (GHVelocity)(WiiExt::_values[2] & 0xE0);
        WiiExt::_ghdrum.isHighHat = !(WiiExt::_values[1] & 0x80);
        WiiExt::_ghdrum.isVelocity = !(WiiExt::_values[1] & 0x40);
        which = (WiiExt::_values[1] & 0x3E);
        //WiiExt::_ghdrum.analogY = (WiiExt::_values[0] & 0x3F);
        WiiExt::_ghdrum.analogX = (WiiExt::_values[0] & 0x3F);
    }
    
    // velocity present
    if(WiiExt::_ghdrum.isVelocity)
    {
        switch(which)
        {
            case 0x1B:
                if(WiiExt::_ghdrum.isHighHat)
                    WiiExt::_ghdrum.velocityType = GHHH;
                else
                    WiiExt::_ghdrum.velocityType = GHP;
                break;
            case 0x19:
                WiiExt::_ghdrum.velocityType = GHR;
                break;
            case 0x11:
                WiiExt::_ghdrum.velocityType = GHY;
                break;
            case 0x0F:
                WiiExt::_ghdrum.velocityType = GHB;
                break;
            case 0x0E:
                WiiExt::_ghdrum.velocityType = GHO;
                break;
            case 0x12:
                WiiExt::_ghdrum.velocityType = GHG;
                break;
            default:
                WiiExt::_ghdrum.velocityType = GHNV;
                break;
        }
    }
    
    // end request?
    WiiExt::_sendByte(0x00, 0x00);
    return true;
}

char* WiiExt::touchToStr(GHTouch value)
{
    switch(value)
    {
        case GHF1:
            return "Fret 1";
        case GHF12:
            return "Fret 1&2";
        case GHF2:
            return "Fret 2";
        case GHF23:
            return "Fret 2&3";
        case GHF3:
            return "Fret 3";
        case GHF34:
            return "Fret 3&4";
        case GHF4:
            return "Fret 4";
        case GHF45:
            return "Fret 4&5";
        case GHF5:
            return "Fret 5";
        case GHNT:
        default:
            return "No touch";
    }
}

char* WiiExt::velTypeToStr(GHVelType value)
{
    switch(value)
    {
        case GHR:
            return "Red";
        case GHY:
            return "Yellow";
        case GHB:
            return "Blue";
        case GHO:
            return "Orange";
        case GHG:
            return "Green";
        case GHP:
            return "Pedal";
        case GHHH:
            return "High Hat";
        case GHNV:
        default:
            return "No velocity";
    }
}

char* WiiExt::velValueToStr(GHVelocity value)
{
    switch(value)
    {
        case GHVVH:
            return "Very Hard";
        case GHVMH:
            return "Medium Hard";
        case GHVH:
            return "Hard";
        case GHVM:
            return "Medium";
        case GHVS:
            return "Soft";
        case GHVMS:
            return "Medium Soft";
        case GHVVS:
            return "Very Soft";
        case GHVNH:
        default:
            return "No Hit";
    }
}

// send a byte of data to the controller
void WiiExt::_sendByte(byte data, byte location)
{
    Wire.beginTransmission(ADDRESS);

    Wire.write(location);
    Wire.write(data);

    Wire.endTransmission();

    delay(10);
}