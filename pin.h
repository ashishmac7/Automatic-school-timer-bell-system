#ifndef pin_h
#define pin_h


#include<Arduino.h>
#include"Button.h"
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>
#include <DS3231.h>

/********************MANUAL BUTTON****************/
#define MANUAL_BUTTON           A0
/*************************************************/

/********************RELAY************************/
#define RELAY                  13

/********************BUTTON***********************/
#define MENU_BUTTON              5
#define NEXT_BUTTON              4
#define UP_BUTTON                3
#define DOWN_BUTTON              2
/*************************************************/

/***********************LCD***********************/
#define RS                     12
#define EN                     11
#define D4                     10
#define D5                     19
#define D6                     8
#define D7                     7
/**************************************************/

Button MenuButton(MENU_BUTTON, LOW);
Button NextButton(NEXT_BUTTON, LOW);
Button UpButton(UP_BUTTON, LOW);
Button DownButton(DOWN_BUTTON, LOW);
#define MAXALARM 5

//LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //
DS3231  rtc(SDA, SCL);

#endif
