/*
 * WiiExt.cpp - Improved improved Wii Nunchuk library for Arduino
 *
 * Based off project by:
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 * Project URL : http://www.gabrielbianconi.com/projects/ArduinoNunchuck/
 *
 * Based on the following resources:
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
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
            break;
        case Taiko_t:
            return WiiExt::_updateTaiko();
            break;
        case GHGuitar_t:
            return WiiExt::_updateGHGuitar();
            break;
        default:
            break;
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

// send a byte of data to the controller
void WiiExt::_sendByte(byte data, byte location)
{
    Wire.beginTransmission(ADDRESS);

    Wire.write(location);
    Wire.write(data);

    Wire.endTransmission();

    delay(10);
}