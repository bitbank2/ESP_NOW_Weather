//
// rtc_eeprom
//
// Copyright (c) 2019 BitBank Software, Inc.
// Written by Larry Bank
// bitbank@pobox.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <Arduino.h>
#ifndef __AVR_ATtiny85__
#include <Wire.h>
#endif
#include <BitBang_I2C.h>
#include "rtc_eeprom.h"

static int iRTCType;
static int iRTCAddr;
static BBI2C bb;

//
// Read a byte from the EEPROM
//
void eeReadByte(int iAddr, unsigned char *pData)
{
unsigned char ucTemp[4];

  if (iAddr != -1) // set the address
  {
    ucTemp[0] = (unsigned char)(iAddr >> 8);
    ucTemp[1] = (unsigned char)iAddr;
    I2CWrite(&bb, EEPROM_ADDR, ucTemp, 2);
    delay(10);
  }
  // otherwise just read from the last address and auto-increment
  I2CRead(&bb, EEPROM_ADDR, pData, 1);
} /* eeReadByte() */
//
// Read a block of 32 bytes from the given address
// or from the last read address if iAddr == -1
//
void eeReadBlock(int iAddr, unsigned char *pData)
{
unsigned char ucTemp[4];

  if (iAddr != -1) // set the address
  {
    ucTemp[0] = (unsigned char)(iAddr >> 8);
    ucTemp[1] = (unsigned char)iAddr;
    I2CWrite(&bb, EEPROM_ADDR, ucTemp, 2);
  }
  // otherwise just read from the last address and auto-increment
  I2CRead(&bb, EEPROM_ADDR, pData, 32);
} /* eeReadBlock() */
//
// Write a byte to the given address
// or the previous address if iAddr == -1
//
void eeWriteByte(int iAddr, unsigned char ucByte)
{
unsigned char ucTemp[4];

        if (iAddr != -1) // send the address
        {
                ucTemp[0] = (unsigned char)(iAddr >> 8);
                ucTemp[1] = (unsigned char)iAddr;
                ucTemp[2] = ucByte;
                // The first data byte must be written with
                // the address atomically or it won't work
                I2CWrite(&bb, EEPROM_ADDR, ucTemp, 3);
        } // otherwise write from the last address and increment
        else
        {
                I2CWrite(&bb, EEPROM_ADDR, &ucByte, 1);
        }
} /* eeWriteByte() */
//
// Write a block of 32 bytes to the given address
// or from the last read/write address is iAddr == -1
//
void eeWriteBlock(int iAddr, unsigned char *pData)
{
unsigned char ucTemp[34];

        if (iAddr != -1) // send the address
        {
                ucTemp[0] = (unsigned char)(iAddr >> 8);
                ucTemp[1] = (unsigned char)iAddr;
                memcpy(&ucTemp[2], pData, 32);
                I2CWrite(&bb, EEPROM_ADDR, ucTemp, 34);
        } // otherwise write to the last address and increment
        else
        {
                I2CWrite(&bb, EEPROM_ADDR, pData, 32);
        }
} /* eeWriteBlock() */
//
// Turn on the RTC
// returns 1 for success, 0 for failure
//
int rtcInit(int iType, int iSDA, int iSCL, int bWire)
{
uint8_t ucTemp[4];

  if (iType <= RTC_UNKNOWN || iType >= RTC_TYPE_COUNT) // invalid type
     return 0;
  iRTCType = iType;
  if (iRTCType == RTC_DS3231)
     iRTCAddr = RTC_DS3231_ADDR;
  else
     iRTCAddr = RTC_PCF8563_ADDR;

  memset(&bb,0,sizeof(bb));
  bb.iSDA = iSDA;
  bb.iSCL = iSCL;
  bb.bWire = bWire;
  I2CInit(&bb, 100000L); // initialize the bit bang library
  if (iType == RTC_DS3231)
  {
    ucTemp[0] = 0xe; // control register
    ucTemp[1] = 0x1c; // enable main oscillator and interrupt mode for alarms
    I2CWrite(&bb, iRTCAddr, ucTemp, 2);
  }
  else // PCF8563
  {
    ucTemp[0] = 0; // control_status_1
    ucTemp[1] = 0; // normal mode, clock on, power-on-reset disabled
    ucTemp[2] = 0; // disable all alarms
    I2CWrite(&bb, iRTCAddr, ucTemp, 3);
  }
  return 1;
}
//
// Set Alarm for:
// ALARM_SECOND = Once every second
// ALARM_MINUTE = Once every minute
// ALARM_TIME = When a specific hour:second match
// ALARM_DAY = When a specific day of the week and time match
// ALARM_DATE = When a specific day of the month and time match
//
void rtcSetAlarm(uint8_t type, struct tm *pTime)
{
uint8_t ucTemp[8];

  if (iRTCType == RTC_DS3231)
  {
    switch (type)
    {
      case ALARM_SECOND: // turn on repeating alarm for every second
        ucTemp[0] = 0xe; // control register
        ucTemp[1] = 0x1d; // enable alarm1 interrupt
        I2CWrite(&bb, iRTCAddr, ucTemp, 2);
        ucTemp[0] = 0x7; // starting register for alarm 1
        ucTemp[1] = 0x80; // set bit 7 in the 4 registers to tell it a repeating alarm
        ucTemp[2] = 0x80;
        ucTemp[3] = 0x80;
        ucTemp[4] = 0x80;
        I2CWrite(&bb, iRTCAddr, ucTemp, 5);
        break;
      case ALARM_MINUTE: // turn on repeating alarm for every minute
        ucTemp[0] = 0xe; // control register
        ucTemp[1] = 0x1e; // enable alarm2 interrupt
        I2CWrite(&bb, iRTCAddr, ucTemp, 2);
        ucTemp[0] = 0xb; // starting register for alarm 2
        ucTemp[1] = 0x80; // set bit 7 in the 3 registers to tell it a repeating alarm
        ucTemp[2] = 0x80;
        ucTemp[3] = 0x80;
        I2CWrite(&bb, iRTCAddr, ucTemp, 4);
        break;
      case ALARM_TIME: // turn on alarm to match a specific time
      case ALARM_DAY: // turn on alarm for a specific day of the week
      case ALARM_DATE: // turn on alarm for a specific date
// Values are stored as BCD
        ucTemp[0] = 0x7; // start at register 7
        // seconds
        ucTemp[1] = ((pTime->tm_sec / 10) << 4);
        ucTemp[1] |= (pTime->tm_sec % 10);
        // minutes
        ucTemp[2] = ((pTime->tm_min / 10) << 4);
        ucTemp[2] |= (pTime->tm_min % 10);
        // hours (and set 24-hour format)
        ucTemp[3] = ((pTime->tm_hour / 10) << 4);
        ucTemp[3] |= (pTime->tm_hour % 10);
        // day of the week
        if (type == ALARM_DAY)
           ucTemp[4] = pTime->tm_wday + 1;
        // day of the month
        else if (type == ALARM_DATE) {
          ucTemp[4] = (pTime->tm_mday / 10) << 4;
          ucTemp[4] |= (pTime->tm_mday % 10);
        } else {
          ucTemp[4] = 0;
        }
        // set the A1Mx bits (high bits of the 4 registers)
        // for the specific type of alarm
        ucTemp[1] &= 0x7f; // make sure A1M1 & A1M2 are set to 0
        ucTemp[2] &= 0x7f;
        if (type == ALARM_TIME) // A1Mx bits should be x1000
        {
          ucTemp[3] &= 0x7f;
          ucTemp[4] |= 0x80;
        }
        else if (type == ALARM_DAY) // A1Mx bits should be 10000
        {
          ucTemp[3] &= 0x7f;
          ucTemp[4] &= 0x7f;
          ucTemp[4] |= 0x40; // DY/DT bit
        }
        // for matching the date, all bits are left as 0's (00000)
        I2CWrite(&bb, iRTCAddr, ucTemp, 5);
        ucTemp[0] = 0xe; // control register
        ucTemp[1] = 0x5; // enable alarm1 interrupt
        ucTemp[2] = 0x00; // reset alarm status bits
        I2CWrite(&bb, iRTCAddr, ucTemp, 3);
        break;
     } // switch on type
  }
  else if (iRTCType == RTC_PCF8563)
  {
    switch (type)
    {
      case ALARM_SECOND: // turn on repeating alarm for every second
        ucTemp[0] = 0x1; // control_status_2
        ucTemp[1] = 0x5; // enable timer & interrupt
        I2CWrite(&bb, iRTCAddr, ucTemp, 2);
        ucTemp[0] = 0xe; // timer control
        ucTemp[1] = 0x81; // enable timer for 1/64 second interval
        ucTemp[2] = 0x40; // timer count value (64 = 1 second)
        I2CWrite(&bb, iRTCAddr, ucTemp, 3);
        break;
      case ALARM_MINUTE: // turn on repeating timer for every minute
        ucTemp[0] = 0x1; // control_status_2
        ucTemp[1] = 0x5; // enable timer & interrupt
        I2CWrite(&bb, iRTCAddr, ucTemp, 2);
        ucTemp[0] = 0xe; // timer control
        ucTemp[1] = 0x82; // enable timer for 1 hz interval
        ucTemp[2] = 0x3c; // 60 = 1 minute
        I2CWrite(&bb, iRTCAddr, ucTemp, 3);
        break;
      case ALARM_TIME: // turn on alarm to match a specific time
      case ALARM_DAY: // turn on alarm for a specific day of the week
      case ALARM_DATE: // turn on alarm for a specific date
        ucTemp[0] = 0x1; // control_status_2
        ucTemp[1] = 0xa; // enable alarm & interrupt
        I2CWrite(&bb, iRTCAddr, ucTemp, 2);
// Values are stored as BCD
        ucTemp[0] = 0x9; // start at register 9
        // seconds
        ucTemp[1] = ((pTime->tm_sec / 10) << 4);
        ucTemp[1] |= (pTime->tm_sec % 10);
        ucTemp[1] |= 0x80; // disable
        // minutes
        ucTemp[2] = ((pTime->tm_min / 10) << 4);
        ucTemp[2] |= (pTime->tm_min % 10);
        ucTemp[2] |= 0x80; // disable
        // hours (and set 24-hour format)
        ucTemp[3] = ((pTime->tm_hour / 10) << 4);
        ucTemp[3] |= (pTime->tm_hour % 10);
        ucTemp[3] |= 0x80; // disable
        // day of the week
        ucTemp[5] = pTime->tm_wday + 1;
        ucTemp[5] = 0x80; // disable
        // day of the month
        ucTemp[4] = (pTime->tm_mday / 10) << 4;
        ucTemp[4] |= (pTime->tm_mday % 10);
        ucTemp[4] |= 0x80; // disable
        // clear high bits of the 4 registers
        // for the specific type of alarm
        if (type == ALARM_TIME)
        {
          ucTemp[1] &= 0x7f;
          ucTemp[2] &= 0x7f;
          ucTemp[3] &= 0x7f;
        }
        else if (type == ALARM_DAY)
        {
          ucTemp[5] &= 0x7f;
        }
        else if (type == ALARM_DATE)
        {
          ucTemp[4] &= 0x7f;
        }
        I2CWrite(&bb, iRTCAddr, ucTemp, 6);
        break;
     } // switch on alarm type
   } // PCF8563
} /* rtcSetAlarm() */
//
// Read the current internal temperature
// Value is celcius * 4 (resolution of 0.25C)
//
int rtcGetTemp(void)
{
unsigned char ucTemp[2];
int iTemp = 0;

  if (iRTCType == RTC_DS3231)
  {
    I2CReadRegister(&bb, iRTCAddr, 0x11, ucTemp, 2); // MSB location
    iTemp = ucTemp[0] << 8; // high byte
    iTemp |= ucTemp[1]; // low byte
    iTemp >>= 6; // lower 2 bits are fraction; upper 8 bits = integer part
    return iTemp;
  }
  return 0; // no temperature sensor
} /* rtcGetTemp() */
//
// Set the current time/date
//
void rtcSetTime(struct tm *pTime)
{
unsigned char ucTemp[20];
uint8_t i;

   if (iRTCType == RTC_DS3231)
   {
// Values are stored as BCD
        ucTemp[0] = 0; // start at register 0
        // seconds
        ucTemp[1] = ((pTime->tm_sec / 10) << 4);
        ucTemp[1] |= (pTime->tm_sec % 10);
        // minutes
        ucTemp[2] = ((pTime->tm_min / 10) << 4);
        ucTemp[2] |= (pTime->tm_min % 10);
        // hours (and set 24-hour format)
        ucTemp[3] = ((pTime->tm_hour / 10) << 4);
        ucTemp[3] |= (pTime->tm_hour % 10);
        // day of the week
        ucTemp[4] = pTime->tm_wday + 1;
        // day of the month
        ucTemp[5] = (pTime->tm_mday / 10) << 4;
        ucTemp[5] |= (pTime->tm_mday % 10);
        // month + century
        i = pTime->tm_mon+1; // 1-12 on the RTC
        ucTemp[6] = (i / 10) << 4;
        ucTemp[6] |= (i % 10);
        if (pTime->tm_year >= 100)
           ucTemp[6] |= 0x80; // century bit
        // year
        ucTemp[7] = (((pTime->tm_year % 100)/10) << 4);
        ucTemp[7] |= (pTime->tm_year % 10);
    }
    else if (iRTCType == RTC_PCF8563)
    {
        ucTemp[0] = 2; // start at register 2
        // seconds
        ucTemp[1] = ((pTime->tm_sec / 10) << 4);
        ucTemp[1] |= (pTime->tm_sec % 10);
        // minutes
        ucTemp[2] = ((pTime->tm_min / 10) << 4);
        ucTemp[2] |= (pTime->tm_min % 10);
        // hours (and set 24-hour format)
        ucTemp[3] = ((pTime->tm_hour / 10) << 4);
        ucTemp[3] |= (pTime->tm_hour % 10);
        // day of the week
        ucTemp[5] = pTime->tm_wday + 1;
        // day of the month
        ucTemp[4] = (pTime->tm_mday / 10) << 4;
        ucTemp[4] |= (pTime->tm_mday % 10);
        // month + century
        i = pTime->tm_mon+1; // 1-12 on the RTC
        ucTemp[6] = (i / 10) << 4;
        ucTemp[6] |= (i % 10);
        if (pTime->tm_year >= 100)
           ucTemp[6] |= 0x80; // century bit
        // year
        ucTemp[7] = (((pTime->tm_year % 100)/10) << 4);
        ucTemp[7] |= (pTime->tm_year % 10);
    }
    I2CWrite(&bb, iRTCAddr, ucTemp, 8);

} /* rtcSetTime() */

//
// Read the current time/date
//
void rtcGetTime(struct tm *pTime)
{
unsigned char ucTemp[20];

  if (iRTCType == RTC_DS3231)
  {
        I2CReadRegister(&bb, iRTCAddr, 0, ucTemp, 7); // start of data registers
        memset(pTime, 0, sizeof(struct tm));
        // convert numbers from BCD
        pTime->tm_sec = ((ucTemp[0] >> 4) * 10) + (ucTemp[0] & 0xf);
        pTime->tm_min = ((ucTemp[1] >> 4) * 10) + (ucTemp[1] & 0xf);
        // hours are stored in 24-hour format in the tm struct
        if (ucTemp[2] & 64) // 12 hour format
        {
                pTime->tm_hour = ucTemp[2] & 0xf;
                pTime->tm_hour += ((ucTemp[2] >> 4) & 1) * 10;
                pTime->tm_hour += ((ucTemp[2] >> 5) & 1) * 12; // AM/PM
        }
        else // 24 hour format
        {
                pTime->tm_hour = ((ucTemp[2] >> 4) * 10) + (ucTemp[2] & 0xf);
        }
        pTime->tm_wday = ucTemp[3] - 1; // day of the week (0-6)
        // day of the month
        pTime->tm_mday = ((ucTemp[4] >> 4) * 10) + (ucTemp[4] & 0xf);
        // month
        pTime->tm_mon = (((ucTemp[5] >> 4) & 1) * 10 + (ucTemp[5] & 0xf)) -1; // 0-11
        pTime->tm_year = (ucTemp[5] >> 7) * 100; // century
        pTime->tm_year += ((ucTemp[6] >> 4) * 10) + (ucTemp[6] & 0xf);
  }
  else if (iRTCType == RTC_PCF8563)
  {
        I2CReadRegister(&bb, iRTCAddr, 2, ucTemp, 7); // start of data registers
        memset(pTime, 0, sizeof(struct tm));
        // convert numbers from BCD
        pTime->tm_sec = (((ucTemp[0] >> 4) & 7) * 10) + (ucTemp[0] & 0xf);
        pTime->tm_min = ((ucTemp[1] >> 4) * 10) + (ucTemp[1] & 0xf);
        // hours are stored in 24-hour format in the tm struct
        pTime->tm_hour = ((ucTemp[2] >> 4) * 10) + (ucTemp[2] & 0xf);
        pTime->tm_wday = ucTemp[4] - 1; // day of the week (0-6)
        // day of the month
        pTime->tm_mday = ((ucTemp[3] >> 4) * 10) + (ucTemp[3] & 0xf);
        // month
        pTime->tm_mon = (((ucTemp[5] >> 4) & 1) * 10 + (ucTemp[5] & 0xf)) -1; // 0-11
        pTime->tm_year = (ucTemp[5] >> 7) * 100; // century
        pTime->tm_year += ((ucTemp[6] >> 4) * 10) + (ucTemp[6] & 0xf);
  }
} /* rtcGetTime() */
//
// Reset the "fired" bits for Alarm 1 and 2
// Interrupts will not occur until these bits are cleared
//
void rtcClearAlarms(void)
{
uint8_t ucTemp[4];

  if (iRTCType == RTC_DS3231)
  {
    ucTemp[0] = 0xe; // control register
    ucTemp[1] = 0x4; // disable alarm interrupt bits
    ucTemp[2] = 0x0; // clear A1F & A2F (alarm 1 or 2 fired) bit to allow it to fire again
    I2CWrite(&bb, iRTCAddr, ucTemp, 3);
  }
  else if (iRTCType == RTC_PCF8563)
  {
    ucTemp[0] = 1; // control_status_2
    ucTemp[1] = 0; // disable all alarms
    I2CWrite(&bb, iRTCAddr, ucTemp, 2);
  }
} /* rtcClearAlarms() */

