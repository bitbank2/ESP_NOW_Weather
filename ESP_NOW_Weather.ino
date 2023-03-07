//
// ESP-NOW e-paper weather project
// Copyright 2023 BitBank Software, Inc. All Rights Reserved.
// started March 5, 2023
// written by Larry Bank
// bitbank@pobox.com
//
// The goal of this project is to experiment with ESP32 networking
// on a battery operated device to use the least power possible
// by leveraging 2 devices - one connected to mains power and the
// other powered by a battery. The mains power (server) will access
// the internet, gather useful data and parse it into a compact
// representation to send to the client using ESP-NOW.
// In this case, weather data from wttr.in is read as JSON (> 60K)
// and parsed into a 36 byte structure sent in a single packet.
//
// The hardware:
// I chose an AtomS3 as the server side since it's small and has
// a built-in color LCD display to show some useful info. For the
// client side, I chose the LaskaKit ESPInk board since it has a
// very low sleep current (~14uA) and can last a long time on a
// 3.7V LiPo battery. I also added an optional SCD4x CO2 sensor
// on the server side. This project supports 2 different sizes of
// e-paper - 2.9" black/white or black/white/red and 4.2"
// BW or BWR. The OneBitDisplay library supports many more types
// of e-paper panels, but the graphics of this project were laid
// out in a fixed format for those two sizes only.

// The software:
// I used the WiFiManager library to request and save the user's
// WiFi credentials. It makes it easy to set up WiFi on any ESP32
// even if it lacks a display. In this case, I provide some
// instructions to the user on the LCD to get them started.
// The LCD, e-paper and SCD4x libraries were written by me:
// https://github.com/bitbank2/SCD41.git
// https://github.com/bitbank2/bb_spi_lcd.git
// https://github.com/bitbank2/OneBitDisplay.git
//
// Operating method:
// The AtomS3 runs continuously and updates the CO2/temp/humidity
// values every 5 seconds since this is the SCD4x update rate.
// Every 43 minutes, it requests the latest weather data
// from wttr.in. This service is completely free and does not
// require registration. The JSON is parsed and the "useful"
// values are placed in my custom weather structure.
// The ESPInk board is asleep most of the time, but wakes
// once per hour. It sends a broadcast packet of 2 bytes to
// request the latest data. The AtomS3 receives the broadcast
// packet and responds with the latest weather structure.
// Neither side needs to the other's MAC address beforehand.
// The e-paper is updated with the new info and the ESPInk
// board goes back to sleep for another hour. The total active
// time of the esp32 is around 100ms plus the time waiting for
// the e-paper to finish updating which uses light sleep.
//
// Building the project:
// To build for the AtomS3 server, simply set the Arduino target
// type to "M5Stack AtomS3". To build for the ESPInk board,
// set the target to "ESP32 Dev Module". #ifdef's are used to
// build the client and server code depending on the target chosen.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//===========================================================================
//
#include <esp_now.h>
#include <WiFi.h>
#if defined( ARDUINO_M5Stack_ATOMS3 ) || defined(ARDUINO_M5Stick_C)
#define SERVER_SIDE
#include <bb_spi_lcd.h>
#include <esp_wifi.h>
#include <SCD41.h>
#include <NTPClient.h>           //https://github.com/taranais/NTPClient
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
HTTPClient http;
BB_SPI_LCD lcd;
SCD41 mySensor;
bool bHasSCD4x = false;
const char url[]= "https://wttr.in/?format=j1";
#ifdef ARDUINO_M5Stick_C
#define LCD_TYPE DISPLAY_M5STACK_STICKCPLUS
// GROVE I2C
#define SDA_PIN 32
#define SCL_PIN 33
#define BITBANG 1
#define BUTTON_USER 39
#define RTC_SDA_PIN 21 
#define RTC_SCL_PIN 22
#include "rtc_eeprom.h"
const bool bHasRTC = true;
#else // AtomS3
#define LCD_TYPE DISPLAY_M5STACK_ATOMS3
// GROVE I2C
#define SDA_PIN 2
#define SCL_PIN 1
#define RTC_SDA_PIN -1
#define RTC_SCL_PIN -1
#define BITBANG 1
#define BUTTON_USER 41
const bool bHasRTC = false;
#endif // AtomS3/STICKC_Plus

#else // e-paper controller
#include <OneBitDisplay.h>
ONE_BIT_DISPLAY obd;
#include "emoji0.h"
#include "emoji1.h"
#include "emoji2.h"
#include "emoji3.h"
#include "emoji4.h"
const uint8_t *pEmojis[5] = {emoji0, emoji1, emoji2, emoji3, emoji4};
#include "Roboto_Black_40.h"
#include "Roboto_Black_20.h"

#ifdef ARDUINO_FEATHERS3
// Feather S3 is connected to a ST7302 low power LCD
// for displaying the time and CO2/Temp/Humidity
#define LCD_FREQ 8000000
#define CS_PIN 7
#define DC_PIN 3
#define RST_PIN 1
#include "Roboto_Black_80.h"

#else // LASKAKIT_ESPINK

#include "Roboto_Black_50.h"
#include "Roboto_Black_28.h"
#include "Roboto_25.h"
// black and white graphics
#include "humidity.h"
#include "sunrise.h"
#include "sunset.h"
#include "wind.h"
#include "temp_bw.h"
#include "rain.h"
#include "uv_icon.h"
#include "hand.h"
#include "co2_icon.h"
static uint8_t ucBuffer[(400*320)/4]; // 2 bit planes for black and red
// LaskaKit ESPInk settings
#define LASKA_KIT
#define CS_PIN 5
#define DC_PIN 17
#define BUSY_PIN 4
#define RST_PIN 16
#define MOSI_PIN 23
#define SCK_PIN 18
#define EPD_TYPE EPD42R2_400x300
#define POWER_PIN 2
#define EPD_FREQ 8000000
#endif // !FEATHERS3
#endif // !AtomS3

// Compact representation of current conditions
typedef struct tagWeather
{
  uint16_t u16CO2;
  int16_t i16IndoorTemp; // 10x Celcius to store as int (e.g. 25.5 = 255)
  int16_t i16OutdoorTemp;
  uint8_t u8IndoorHumidity;
  uint8_t u8OutdoorHumidity;
  uint32_t u32CurrentTime; // current GMT epoch time
  uint32_t u32WeatherTime; // GMT time the weather info was last gathered
  uint8_t u8SunsetHour;
  uint8_t u8SunsetMin;
  uint8_t u8SunriseHour;
  uint8_t u8SunriseMin;
  uint8_t u8UV;
  int8_t i8LowTemp; // whole digit C is good enough for forecast
  int8_t i8HighTemp;
  int8_t i8FeelsTemp;
  uint8_t u8Wind;
  uint8_t u8RainToday[8]; // 3 hour segments of rain probability starting at midnight
  uint8_t u8RainTomorrow[8];
} WEATHER;

// MAC Address of the receiver
uint8_t u8BroadcastAddr[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t u8PeerAddr[6];
WEATHER myWeather;

esp_now_peer_info_t peerInfo;
volatile bool bGotResponse = false;
volatile int iDataLen;
static uint8_t ucPacket[250];

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 // Serial.print("\r\nLast Packet Send Status:\t");
 // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//  if (status ==0){
//    success = "Delivery Success :)";
//  }
//  else{
//    success = "Delivery Fail :(";
//  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(ucPacket, incomingData, len);
    iDataLen = len;
    bGotResponse = true;
    memcpy(u8PeerAddr, mac, 6); // we need the sender's MAC so that we can respond
} /* onDataRecv() */

void RFBegin()
{
// Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
// Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
// Once ESPNow is successfully Init, we will register for Send CB to
  // register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
} /* RFBegin() */

void RFEnd(void)
{
  esp_now_deinit();
  WiFi.mode(WIFI_OFF);
} /* RFEnd() */

#ifndef SERVER_SIDE
int batteryVoltage(void)
{
int i;
float v;

#if defined(ARDUINO_ESP32C3_DEV) // Mike's ESP32-C3 1.54" e-ink board
  i = analogRead(0);
  v = ((float)i / 4096.0f) * 5.83325f;      // 100K+100K voltage divider
#elif defined(LASKA_KIT)
  i = analogRead(34);
  v = 4.00f + ((float)(i-3783)) / 2000.0f;
#elif defined(ARDUINO_TINYS3)
      esp_adc_cal_characteristics_t adc_cal;
      esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_12, 0, &adc_cal);
      adc1_config_channel_atten(ADC1_GPIO10_CHANNEL, ADC_ATTEN_DB_2_5);

      i = adc1_get_raw(ADC1_GPIO10_CHANNEL);
      uint32_t millivolts = esp_adc_cal_raw_to_voltage(i, &adc_cal);
      const uint32_t upper_divider = 442;
      const uint32_t lower_divider = 160;
      v = (float)(upper_divider + lower_divider) / lower_divider / 1000 * millivolts;
#elif defined(ARDUINO_FEATHERS3)
#define VBAT_PIN 2
  i = analogRead(VBAT_PIN);
  v = 3.89f + ((float)(i - 1197)/324.0f);

#else
(void)i;
v = 4.00f; // DEBUG
#endif
  return (int)(v * 1000.0f); // return V in millivolts
} /* batteryVoltage() */

#ifdef ARDUINO_FEATHERS3
void ShowTime(void)
{
struct tm myTime;
char szTemp[128];
int i;

  // Display the latest time we got from the ESP-NOW server
  obd.fillScreen(OBD_WHITE);
  memcpy(&myTime, gmtime ((time_t *)&myWeather.u32CurrentTime), sizeof(myTime));
  sprintf(szTemp,"%02d:%02d", myTime.tm_hour, myTime.tm_min);
//  Serial.println(szTemp);
  obd.setFreeFont(&Roboto_Black_80);
  obd.setCursor(30, 118);
  obd.print(szTemp); // show time

  obd.setFreeFont(&Roboto_Black_20);
  obd.setCursor(8,18);
  obd.print("CO2");
//  lcd.setFreeFont(&Roboto_Black_20);
//  lcd.print("2   ");
  obd.setCursor(72,18);
  sprintf(szTemp, "%d", myWeather.u16CO2);
//  lcd.setFreeFont(&Roboto_Black_40);
  obd.print(szTemp);
  sprintf(szTemp, "%dC", myWeather.i16IndoorTemp/10);
  obd.setCursor(8,36);
  obd.print("Temp");
  obd.setCursor(72, 36);
  obd.print(szTemp);
  sprintf(szTemp, "%d%%", myWeather.u8IndoorHumidity);
  obd.setCursor(8,54);
  obd.print("Hum");
  obd.setCursor(72,54);
  obd.print(szTemp);
  // Display the CO2 emoji appropriate for the current CO2 level
  i = myWeather.u16CO2/750; // each CO2 category is 750ppm
  // 0-749 = 0, 750-1499 = 1, 1500=2249=2, 2250-2999=3, 3000+ = 4
  if (i < 0) i = 0;
  else if (i > 4) i = 4; 
  obd.loadBMP((uint8_t *)pEmojis[i], 140, 8, OBD_WHITE, OBD_BLACK);

  obd.display();
} /* ShowTime() */

#else // LASKAKIT ESPINK

void startEPD(void)
{
  if (POWER_PIN != -1) {
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH); // turn on LDO2 (power to EPD)
    delay(50); // allow time to power up
  }
  // Initialize OneBitDisplay library for our e-ink panel
  obd.setSPIPins(CS_PIN, MOSI_PIN, SCK_PIN, DC_PIN, RST_PIN, BUSY_PIN);
  obd.SPIbegin(EPD_TYPE, EPD_FREQ);
  obd.setBuffer(ucBuffer);

  if (obd.width() < obd.height())
     obd.setRotation(90);
  obd.fillScreen(OBD_WHITE);
} /* startEPD() */

// Draw the small bar graph of the rain probabilities
// Uses black for < 50% and red for >= 50%
void showRain(int x, int y, int cx, int cy, const char *szLabel, uint8_t *pValues)
{
  int i, j, tx = (cx-2)/8;
    obd.drawRect(x, y, cx, cy, OBD_BLACK);
    for (i=0; i<8; i++) { // 8 values of rain probability for today and tomorrow
      j = (pValues[i]*(cy-2))/100; // height of probability for today
      obd.fillRect(x+1+(i*tx), y+1+(cy-2-j), tx, j, (pValues[i]<50) ? OBD_BLACK : OBD_RED);
    }
    // add label
    obd.setFont(FONT_8x8);
    obd.setCursor(x+(cx/2)-4*strlen(szLabel),y-8);
    obd.print(szLabel);
} /* showRain() */

const char *szMonths[] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
const int iMonthLens[] = {31, 28, 31, 30, 31, 30, 31, 30, 30, 31, 30, 31};
const char *szDays[] = {"S", "M", "T", "W", "T", "F", "S"};
void drawCalendar(struct tm *pTime, const GFXfont *pFont, int x, int y)
{
int dx, dy; // calculate delta of each cell
uint8_t u8LineClr, u8CellClr, u8TextClr;
int tx, ty, iDay, iDeltaX;
int iMonth, iStartDay;
int16_t x1, y1;
uint16_t w, w2, h, h2;

   iMonth = pTime->tm_mon;
   iStartDay = pTime->tm_wday - ((pTime->tm_mday-1) % 7);
   if (iStartDay < 0) iStartDay += 7;
   
   u8LineClr = OBD_RED;
   u8CellClr = OBD_WHITE;
   u8TextClr = OBD_BLACK;

   obd.setFreeFont(pFont);
   obd.getTextBounds("00", 0, 0, &x1, &y1, &w, &h);
   dx = w+3; // width of each cell, not including the surrounding lines
   dy = (h*5)/4; // height of each cell, not including the surrounding lines

   // draw the month name area
   obd.setTextColor(u8TextClr);
   obd.drawRect(x,y,(dx*7)+8,dy+2, u8LineClr);
   obd.fillRect(x+1,y+1,(dx*7)+6,dy-1, u8CellClr);
   obd.getTextBounds(szMonths[iMonth], 0, 0, &x1, &y1, &w2, &h2);
   iDeltaX = 1 + ((dx*7+6) - w2)/2;
   obd.setCursor(x+iDeltaX, y + h2 + (dy-h2)/2);
   obd.print(szMonths[iMonth]);
   
   // draw the grid and cells
   for (int j=1; j<7; j++) { // y
     for (int i=0; i<7; i++) { // x
        obd.drawRect(x+(i*(dx+1)),y+(j*(dy+1)),dx+2,dy+2, u8LineClr);
     }
   } // for j
   // draw the day names
   ty = y + dy + 1;
   tx = x + 1;
   for (int i=0; i<7; i++) {
      obd.setCursor(tx + (dx-w/2)/2, ty + h + (dy-h)/2);
      obd.print(szDays[i]);
      tx += dx+1;
   }
   // draw the days of the month
   ty = y + 1 + (dy + 1)*2;
   tx = x + 1 + (iStartDay * (dx+1));
   iDay = iStartDay;
   for (int i=1; i<=iMonthLens[iMonth]; i++) {
      uint8_t u8Clr;
      iDeltaX = (i < 10) ? (dx-w/2)/2 : (dx-w)/2;
      u8Clr = (i == pTime->tm_mday) ? u8TextClr : u8CellClr;
      obd.fillRect(tx,ty,dx,dy, u8Clr);
      u8Clr = (i != pTime->tm_mday) ? u8TextClr : u8CellClr;
      obd.setTextColor(u8Clr);
      obd.setCursor(tx + iDeltaX, ty + h + (dy-h)/2);
      obd.print(i,DEC);
      tx += dx+1;
      iDay++;
      if (iDay == 7) { // next line
        iDay = 0;
        tx = x + 1;
        ty += dy+1;
      }
   }
} /* drawCalendar() */

void ShowWeather(void)
{
  char szTemp[128];
  int i, j;

  startEPD();
  // show the battery voltage
  if (obd.width() < 400) {
    obd.setFont(FONT_6x8);
    j = 6;
  } else {
    obd.setFont(FONT_8x8);
    j = 8;
  }
  obd.setCursor(0, obd.height()-8);
  obd.print("vbatt ");
  i = batteryVoltage(); // millivolts
  obd.setTextColor((i < 3600) ? OBD_RED : OBD_BLACK); // show red text when battery is low
  sprintf(szTemp, "%01d.%03d",i/1000, i % 1000); 
  obd.print(szTemp);

  // show the update time+date
  struct tm *ptm = gmtime ((time_t *)&myWeather.u32WeatherTime);
  sprintf(szTemp, "Updated %02d/%02d/%04d %02d:%02d", ptm->tm_mday, ptm->tm_mon+1, ptm->tm_year+1900, ptm->tm_hour, ptm->tm_min);
  obd.setCursor(obd.width() - (j*strlen(szTemp)),obd.height()-8);
  obd.setTextColor(OBD_BLACK, OBD_WHITE);
  obd.print(szTemp);
  if (obd.width() < 400) { // Small display layout
    // display sunrise+sunset times first
    obd.loadBMP((uint8_t *)sunrise,0,0,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)sunset,0,40,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)co2_icon,188,28,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)humidity, 0, 76, OBD_RED, OBD_WHITE);
//    obd.loadBMP((uint8_t *)humidity,0,78,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)wind,108,0,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)temp_bw,108,40,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)rain,100,82,OBD_RED, OBD_WHITE);
    obd.loadBMP((uint8_t *)uv_icon,232,0,OBD_RED, OBD_WHITE);
    obd.setTextColor(OBD_BLACK, OBD_WHITE);
    sprintf(szTemp, "%d:%02d", myWeather.u8SunriseHour, myWeather.u8SunriseMin);
    obd.setCursor(48,28);
    //obd.setFont(FONT_12x16);
    obd.setFreeFont(&Roboto_25);
    obd.print(szTemp);
    sprintf(szTemp, "%d:%02d", myWeather.u8SunsetHour, myWeather.u8SunsetMin);
    obd.setCursor(48,68);
    obd.print(szTemp);
    obd.setCursor(34,104);
    obd.print(myWeather.u8OutdoorHumidity, DEC);
    obd.setCursor(236,68);
    obd.print(myWeather.u16CO2, DEC);
    obd.setCursor(148,28);
    obd.print(myWeather.u8Wind, DEC);
    obd.print(" kph");
    obd.setCursor(140,60);
    sprintf(szTemp, "%dC", myWeather.i16OutdoorTemp);
    obd.print(szTemp);
    sprintf(szTemp, "(%d/%d)", myWeather.i8LowTemp, myWeather.i8HighTemp);
    obd.setFont(FONT_8x8); // squeeze in this info in a smaller font
    obd.setCursor(136,64);
    obd.print(szTemp);
    obd.setCursor(234,60);
    //obd.setFont(FONT_12x16);
//    obd.setFreeFont(&Roboto_25);
//    obd.print(myWeather.i8FeelsTemp, DEC);
//    obd.print("C");
    obd.setCursor(272,28);
    obd.setFreeFont(&Roboto_25);
    obd.print(myWeather.u8UV,DEC); // show max UV index
    // Show rain chance as a bar graphs for today and tomorrow
    showRain(144, 89, 74, 27, "today", myWeather.u8RainToday);
    showRain(220, 89, 74, 27, "tomorrow", myWeather.u8RainTomorrow);
    obd.display();
    return;
  } else { // 4.2" display
    struct tm *ptm = gmtime ((time_t *)&myWeather.u32CurrentTime);
    drawCalendar(ptm, &Roboto_Black_20, 90, 148);
    // display sunrise+sunset times first
    obd.loadBMP((uint8_t *)sunrise,8,0,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)sunset,8,48,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)wind,180,0,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)humidity,180,48,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)uv_icon,292,48,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)temp_bw,8,96,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)hand,300,96,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)rain,24,140,OBD_WHITE, OBD_RED);
    obd.loadBMP((uint8_t *)co2_icon, 332, 140, OBD_WHITE, OBD_RED);
    // Show rain chance as a bar graphs for today and tomorrow
    showRain(8, 192, 74, 32, "today", myWeather.u8RainToday);
    showRain(8, 242, 74, 32, "tomorrow", myWeather.u8RainTomorrow);
    sprintf(szTemp, "%d:%02d", myWeather.u8SunriseHour, myWeather.u8SunriseMin);
    obd.setCursor(60,36);
    obd.setFreeFont(&Roboto_Black_40);
    obd.print(szTemp);
    sprintf(szTemp, "%d:%02d", myWeather.u8SunsetHour, myWeather.u8SunsetMin);
    obd.setCursor(60,80);
    obd.print(szTemp);
    obd.setCursor(220,80);
    obd.print(myWeather.u8OutdoorHumidity, DEC);
    obd.setCursor(306, 228);
    obd.print(myWeather.u16CO2, DEC);
    // Display the CO2 emoji appropriate for the current CO2 level
    j = myWeather.u16CO2/750; // each CO2 category is 750ppm
    // 0-749 = 0, 750-1499 = 1, 1500=2249=2, 2250-2999=3, 3000+ = 4
    if (j < 0) j = 0;
    else if (j > 4) j = 4; 
    obd.loadBMP((uint8_t *)pEmojis[j], 328, 232, OBD_WHITE, OBD_RED);
    obd.setCursor(228,36);
    obd.print(myWeather.u8Wind, DEC);
    obd.print(" kph");
    obd.setCursor(40,132);
    sprintf(szTemp, "%dC (%d/%d)", myWeather.i16OutdoorTemp, myWeather.i8LowTemp, myWeather.i8HighTemp);
    obd.print(szTemp);
    obd.setCursor(340, 132);
    obd.print(myWeather.i8FeelsTemp, DEC);
    obd.setFreeFont(&Roboto_Black_40);
    obd.setCursor(340,80);
    obd.print(myWeather.u8UV,DEC); // show max UV index
    obd.display();
    return;
  }
} /* ShowWeather() */
#endif // !ARDUINO_FEATHERS3
#endif // !AtomS3

#ifdef SERVER_SIDE
//
// Use the WiFiManager to ask the user for their credentials
// and then connect without asking in future sessions
//
void ConnectWiFi(void)
{
WiFiManager wm;
bool res;

  //wm.resetSettings(); // DEBUG

  // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "E_Paper_AP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result
    wm.setEnableConfigPortal(false); // we want to know if connecting fails to inform user of what they're expected to do
    wm.setConfigPortalTimeout(120); // keep active for 2 minutes
    res = wm.autoConnect("AtomS3"); // not password protected
    if (!res) { // failed to connect or no saved credentials, start the config portal
try_again:
      lcd.setFont(FONT_6x8);
      lcd.setTextColor(0xffff, 0);
      lcd.println("WiFi conn failed");
      lcd.println("or no credentials");
      lcd.println("Use AtomS3/PW123456");
      lcd.println("and navigate to:");
      lcd.println("http://192.168.4.1");
      lcd.println("To configure future");
      lcd.println("automatic connections");
      lcd.print("timeout in 120 sec...");
      res = wm.startConfigPortal("AtomS3", "PW123456");
      if (!res) {
          lcd.fillScreen(0);
          lcd.println("Portal timed out");
          lcd.println("Press button to");
          lcd.println("try again");
          while (digitalRead(BUTTON_USER) == HIGH) {
            vTaskDelay(1);
          }
          goto try_again;
      } else {
          lcd.fillScreen(0);
          lcd.setFont(FONT_8x8);
          lcd.setCursor(0,48);
          lcd.setTextColor(0x7e0,0);
          lcd.println("Connect WiFi\nSuccess!");
      }
    }
} /* ConnectWiFi() */
//
// Update current time and weather info
//
void GetNewData(void)
{
  int iTimeout = 0;
  int httpCode = -1;
  struct tm myTime;
  lcd.setCursor(0,48);
  lcd.setTextColor(0x7e0,0);
  lcd.println("Connect WiFi");
  ConnectWiFi();
  lcd.setFont(FONT_8x8);
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(0);  // GMT
  timeClient.update();
  myWeather.u32WeatherTime = timeClient.getEpochTime();
  timeClient.end(); // no longer needed
  if (bHasRTC) {
    // set the RTC from the NTP time
    memcpy(&myTime, gmtime ((time_t *)&myWeather.u32WeatherTime), sizeof(myTime));
    rtcSetTime(&myTime);
  }
  // Get the weather info
   http.begin(url);
   httpCode = http.GET();  //send GET request
   if (httpCode != 200) {
     http.end();
   } else {
     char *s;
     String payload = http.getString();
     http.end();
     DynamicJsonDocument doc(26000); // hopefully this is enough to capture the data; latest request returns 48k of text
     DeserializationError err = deserializeJson(doc, payload);
     doc.shrinkToFit();
     if (err == 0) {
        int uvIndex[8];
        int i, j;
        char szTemp[32];
        JsonArray cca = doc["current_condition"].as<JsonArray>();
        JsonObject current_condition = cca[0];
//       JsonArray wd = current_condition["weatherDesc"].as<JsonArray>();
//       JsonObject weatherdesc = wd[0];
        myWeather.u8OutdoorHumidity = (uint8_t)current_condition["humidity"];
        myWeather.i8FeelsTemp = (int8_t)current_condition["FeelsLikeC"];
//     updated = String((const char *)current_condition["localObsDateTime"]); // last update time
        myWeather.i16OutdoorTemp = (int8_t)current_condition["temp_C"];
        myWeather.u8Wind = (uint8_t)current_condition["windspeedKmph"];
 //    desc = String((const char *)weatherdesc["value"]);
       //cc_icon = current_condition["weatherCode"]; 
       JsonArray wa = doc["weather"].as<JsonArray>();
       JsonObject weather = wa[0];
       JsonArray aa = weather["astronomy"].as<JsonArray>();
       JsonArray ha = weather["hourly"].as<JsonArray>();
       JsonObject astronomy = aa[0];
       String sSunrise = String((const char *)astronomy["sunrise"]);
       String sSunset = String((const char *)astronomy["sunset"]);
       strcpy(szTemp, sSunrise.c_str());
       szTemp[2] = 0;
       szTemp[5] = 0;
       myWeather.u8SunriseHour = atoi(szTemp);
       myWeather.u8SunriseMin = atoi(&szTemp[3]);
       strcpy(szTemp, sSunset.c_str());
       szTemp[2] = 0;
       szTemp[5] = 0;
       myWeather.u8SunsetHour = atoi(szTemp);
       myWeather.u8SunsetMin = atoi(&szTemp[3]);

       myWeather.i8LowTemp = (int8_t)weather["mintempC"];
       myWeather.i8HighTemp = (int8_t)weather["maxtempC"];
       // get hourly info for today
       for (i=0; i<8; i++) {
         //iTemp[i] = ha[i]["tempC"];
         uvIndex[i] = (uint8_t)ha[i]["uvIndex"];
         //iHumidity[i] = ha[i]["humidity"];
         //iWeatherCode[i] = ha[i]["weatherCode"];
         myWeather.u8RainToday[i] = ha[i]["chanceofrain"];
       } // for i
       j = 0;
       for (i=0; i<8; i++) {
         if (j < uvIndex[i]) j = uvIndex[i]; // get max UV index for the day
       }
       myWeather.u8UV = (uint8_t)j;

       // get hourly info for tomorrow
       weather = wa[1];
       ha = weather["hourly"].as<JsonArray>();
       for (int i=0; i<8; i++) {
         //iTemp[i+8] = ha[i]["tempC"];
         //iHumidity[i+8] = ha[i]["humidity"];
         //iWeatherCode[i+8] = ha[i]["weatherCode"];
         myWeather.u8RainTomorrow[i] = ha[i]["chanceofrain"];
       } // for i
       lcd.setCursor(0,64);
       lcd.setTextColor(0x1f,0);
       lcd.println("Latest Weather");
       lcd.setTextColor(0x7e0,0);
       struct tm *ptm = gmtime ((time_t *)&myWeather.u32WeatherTime);
       lcd.printf("%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
     } else { // got weather info
       lcd.setCursor(0,64);
       lcd.setTextColor(0x1f,0);
       lcd.println("Latest Weather");
       lcd.setTextColor(0xf800,0);
       lcd.print("Failed     ");
     }
   }
   WiFi.disconnect(true);
   WiFi.mode(WIFI_OFF); 
   esp_wifi_deinit(); // free memory
} /* GetNewData() */
#endif // ATOMS3

void lightSleep(uint64_t time_in_ms)
{
  esp_sleep_enable_timer_wakeup(time_in_ms * 1000L);
  esp_light_sleep_start();
}

void deepSleep(uint64_t time_in_ms)
{
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); // hibernation mode - only RTC powered
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);
  esp_sleep_enable_timer_wakeup(time_in_ms * 1000L);
//  if (POWER_PIN == -1) { // no power cutoff pin, need to hold RESET high while in deep sleep
//    gpio_hold_en((gpio_num_t)RST_PIN);
//    gpio_deep_sleep_hold_en(); 
//  }
  esp_deep_sleep_start();
}

void setup() {

#ifdef SERVER_SIDE
// Weather Server side
  if (bHasRTC) {
    rtcInit(RTC_PCF8563, RTC_SDA_PIN, RTC_SCL_PIN, 1);
  }
  pinMode(BUTTON_USER, INPUT);
  lcd.begin(LCD_TYPE);
  lcd.fillScreen(0);
  lcd.setFont(FONT_8x8);
  lcd.setTextColor(0x1f,0);
  lcd.println("Info Server");
// Initialize the SCD4x
  if (mySensor.init(SDA_PIN, SCL_PIN, BITBANG, 100000) == SCD41_SUCCESS)
  {
      mySensor.start();
      lcd.println("SCD4x started");
      bHasSCD4x = true;
  } else { // can't find the sensor, stop
      lcd.println("SCD4x not found");
      myWeather.i16IndoorTemp = 0;
      myWeather.u8IndoorHumidity = 0;
      myWeather.u16CO2 = 0;
  }
#else
#ifdef ARDUINO_FEATHERS3
  obd.setSPIPins(CS_PIN, -1, -1, DC_PIN, RST_PIN);
  obd.SPIbegin(LCD_ST7302, LCD_FREQ); // 8Mhz is fast enough
  obd.allocBuffer();
  obd.fillScreen(OBD_WHITE);
#endif // FEATHERS3
#endif
} /* setup() */

void loop() {
  long lTime = -1;
  int iTimeout;
  esp_err_t result;
 
 #ifdef SERVER_SIDE
 start_over:
   RFBegin();
   // Register peer
   memcpy(peerInfo.peer_addr, u8BroadcastAddr, 6);
   peerInfo.channel = 0;
   peerInfo.encrypt = false;
  
   // Add peer        
   esp_now_add_peer(&peerInfo);
  // listen for a ping from the battery powered device
   bGotResponse = false;
 while (1) { // do this forever
   vTaskDelay(10);
   lTime++;    
   if (bGotResponse && iDataLen == 2 && ucPacket[0] == 0x55 && ucPacket[1] == 0xaa) {
       // Data request from peer, send a response
         bGotResponse = false; // reset for next time
         memcpy(peerInfo.peer_addr, u8PeerAddr, 6);
         esp_now_add_peer(&peerInfo);
         esp_now_send(u8PeerAddr, (uint8_t *)&myWeather, sizeof(WEATHER)); // send latest weather data
         lcd.setCursor(0,112);
         lcd.setTextColor(0xf81f, 0);
         lcd.print("Sent Data");
   } // data request
   if (bHasSCD4x && (lTime & 0x1ff) == 0) { // 5 seconds
        mySensor.getSample(); // request the next sample
        lcd.setCursor(0,16);
        lcd.setTextColor(0xffff, 0);
        // Display local data
        lcd.printf("CO2 = %d   \n", mySensor.co2());
        lcd.printf("Temp = %dC  \n", mySensor.temperature());
        lcd.printf("Humidity = %d%%  \n", mySensor.humidity());
        // place local data into structure to send remote MCU
        myWeather.i16IndoorTemp = mySensor.temperature() * 10;
        myWeather.u8IndoorHumidity = mySensor.humidity();
        myWeather.u16CO2 = mySensor.co2();
        if (bHasRTC) { // update the current time when we update the CO2 data
          struct tm myTime;
          rtcGetTime(&myTime);
          myWeather.u32CurrentTime = mktime(&myTime);
          lcd.setCursor(0,40);
          lcd.printf("%02d:%02d:%02d", myTime.tm_hour, myTime.tm_min, myTime.tm_sec);          
        }
    } // 5 second CO2 update
  if ((lTime & 0x3ffff) == 0) { // about every 43 minutes
    // Get the correct time and weather data
    GetNewData();
    goto start_over;
  }
} // while (1)

#else // battery powered ESP32 device

// Get the latest weather info from the peer device, then sleep for an hour
  // Send a ping to the "always listening" device
  // and receive a response with useful data
  // Register peer
  RFBegin();
  memcpy(peerInfo.peer_addr, u8BroadcastAddr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // Add peer        
  esp_now_add_peer(&peerInfo);
  ucPacket[0] = 0x55;
  ucPacket[1] = 0xaa; // our msg indicating "send me data"
  bGotResponse = false;
  result = esp_now_send(u8BroadcastAddr, (uint8_t *)ucPacket, 2);
  iTimeout = 0;
  while (iTimeout < 100) { // wait up to a second for a response
    if (bGotResponse) break;
    iTimeout++;
    vTaskDelay(1);
  }
  RFEnd();
  if (bGotResponse) { // display the new data
     memcpy(&myWeather, ucPacket, sizeof(myWeather));
#ifdef ARDUINO_FEATHERS3
     ShowTime();
#else // LASKAKIT ESPINK
     ShowWeather();
#endif
  }
#ifdef ARDUINO_FEATHERS3
  deepSleep(60000); // show new time once per minute
#else
  lightSleep(1000); // allow EPD to finish final cycle
  deepSleep(3600000); // sleep for an hour
#endif // !FEATHERS3
#endif
} /* loop() */
