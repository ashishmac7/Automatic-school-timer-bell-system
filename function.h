#ifndef function_h
#define function_h
#include "pin.h"

enum button_state {NONE, SET, NEXT, UP, DOWN} buttonState; //
/*********************Function Prototype*****************/
void AlarmFunction(void);
void MenuFunction(uint8_t count);
void SetAlarm(uint8_t val);
void  handleClockSet(void );
void ExitMenu(void);
void blink(struct Alarm, int);
button_state HandleButton();
/***********************************************************/


char *dow[8] = {"NONE", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
int nextalarm[2][5];
struct Alarm {
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t duration = 0;
} alarm[2][MAXALARM];
struct clockset {
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t DOW = 1;
} clockSet;

void Menu()
{
  lcd.clear();
  enum state_main {SETALARM, ALARMFORWEEKDAYS, ALARMFORWEEKEND, SETTIME, EXIT} state;
  state = SETALARM;
  char buffer[2][16];
  while (1)
  {
    buttonState = HandleButton();
    switch (state)
    {
      case SETALARM:

        sprintf(buffer[0], "Menu:");
        sprintf(buffer[1], "SET ALARM");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == NEXT)
        {
          state = SETTIME;
          lcd.clear();
        }
        else if (buttonState == SET)
        {
          state = ALARMFORWEEKDAYS;
          lcd.clear();
        }
        break;
      case ALARMFORWEEKDAYS:
        snprintf(buffer[0], sizeof(buffer), "SET ALARM FOR");
        snprintf(buffer[1], sizeof(buffer), "WEEKDAYS");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == SET)
        {
          lcd.clear();
          SetAlarm(0);
        }
        else if (buttonState == NEXT)
        {
          state = ALARMFORWEEKEND;
          lcd.clear();
        }

        break;
      case ALARMFORWEEKEND:
        snprintf(buffer[0], sizeof(buffer), "SET ALARM FOR");
        snprintf(buffer[1], sizeof(buffer), "WEEKEENDS");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
         if (buttonState == SET)
        {
          lcd.clear();
          SetAlarm(1);
        }
        if (buttonState == NEXT)
        {
          state = EXIT;
          lcd.clear();
        }

        break;

      case SETTIME:
        sprintf(buffer[0], "Menu:");
        sprintf(buffer[1], "SET TIME");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == SET)
        {
          lcd.clear();
          handleClockSet();

        }
        else if (buttonState == NEXT)
        {
          state = EXIT;
          lcd.clear();
        }
        break;
      case EXIT:
        sprintf(buffer[0], "Menu:");
        sprintf(buffer[1], "EXIT");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == NEXT)
        {
          state = SETALARM;
          lcd.clear();
        }
        if (buttonState == SET)
        {
          lcd.clear();
          return;
        }
        break;
    }
  }
}

void SetAlarm(uint8_t val)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  char buffer[2][16];
  enum alarm_enum {SETHOUR, SETMIN, SETDUR} alarm_mode;
  alarm_mode = SETHOUR;
  int alarmId = 0;
  EEPROM.get(0, alarm);
  while (1)
  {
    buttonState = HandleButton();
    snprintf(buffer[0], sizeof(buffer[0]), "Set Bell  %02d/%02d", alarmId + 1, MAXALARM);
    lcd.setCursor(0, 0);
    lcd.print(buffer[0]);
    /*snprintf(buffer[1], sizeof(buffer[1]), "%02d:%02d  Dur:%03d s",  alarm[val][alarmId].hour, alarm[val][alarmId].minute, alarm[val][alarmId].duration);
      lcd.setCursor(0, 1);
      lcd.print(buffer[1]);*/
    blink(alarm[val][alarmId], alarm_mode);

    switch (alarm_mode)
    {
      case SETHOUR:
        if (buttonState == UP)
        {
          alarm[val][alarmId].hour++;
          if (alarm[val][alarmId].hour > 23)
            alarm[val][alarmId].hour = 0;
        }
        else if (buttonState == DOWN)
        {
          alarm[val][alarmId].hour--;
          if (alarm[val][alarmId].hour > 23)
            alarm[val][alarmId].hour = 23;
        }
        else  if (buttonState == NEXT)
        {
          alarm_mode = SETMIN;
          lcd.clear();
        }
        break;

      case SETMIN:
        if (buttonState == UP)
        {
          alarm[val][alarmId].minute++;
          if (alarm[val][alarmId].minute > 59)
            alarm[val][alarmId].minute = 0;
        }
        else if (buttonState == DOWN)
        {
          alarm[val][alarmId].minute--;
          if (alarm[val][alarmId].minute > 60)
            alarm[val][alarmId].minute = 59;
        }
        else  if (buttonState == NEXT)
        {
          alarm_mode = SETDUR;
          lcd.clear();
        }
        break;
      case SETDUR:
        if (buttonState == UP)
        {
          alarm[val][alarmId].duration++;
          if (alarm[val][alarmId].duration > 120)
            alarm[val][alarmId].duration = 0;
        }
        else if (buttonState == DOWN)
        {
          alarm[val][alarmId].duration--;
          if (alarm[val][alarmId].duration > 120)
            alarm[val][alarmId].duration = 120;
        }
        else  if (buttonState == NEXT)
        {
          alarm_mode = SETHOUR;
          alarmId++;
          EEPROM.put(0, alarm);
          if (alarmId >= MAXALARM)
          {
            lcd.clear();
            return;
          }
        }
        break;
    }
 
  }
}
void blink(Alarm x, int j)
{
  lcd.setCursor(0, 1);
  char buffer[16];
  static long last_millis = millis();
  if (millis() - last_millis < 1000)
  {
    snprintf(buffer, 16, "%02d:%02d  Dur:%03d s", x.hour, x.minute, x.duration);
    lcd.setCursor(0, 1);
    lcd.print(buffer);
  }
  else
  {
    if (millis() - last_millis < 1200)
    {
      switch (j)
      {
        case 0:
          lcd.setCursor(0, 1);
          snprintf(buffer, 16, "  :%02d  Dur:%03d s",  x.minute, x.duration);
          break;
        case 1:
          lcd.setCursor(0, 1);
          snprintf(buffer, 16, "%02d:    Dur:%03d s", x.hour, x.duration);
          break;
        case 2:
          lcd.setCursor(0, 1);
          snprintf(buffer, 16, "%02d:%02d  Dur:    s", x.hour, x.minute);
          break;
      }
      lcd.print(buffer);
    }
    else
      last_millis = millis();
  }
  //lcd.print(buffer);


}

void  handleClockSet()
{
  lcd.clear();
  enum clockSet_enum {SETDAY, SETHOUR, SETMIN, EXIT} clocksetState;
  clocksetState = SETDAY;
  char buffer[2][16];
  while (1)
  {
    buttonState = HandleButton();
    switch (clocksetState)
    {
      case SETDAY:
        snprintf(buffer[0], sizeof(buffer[0]), "SET DAY");
        snprintf(buffer[1], sizeof(buffer[1]), "%s", dow[clockSet.DOW]);
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == UP)
        {
          clockSet.DOW++;
          if (clockSet.DOW > 7)
            clockSet.DOW = 1;
        }
        else if (buttonState == DOWN)
        {
          clockSet.DOW--;
          if (clockSet.DOW == 0)
            clockSet.DOW = 7;
        }
        else if (buttonState == NEXT)
        {
          clocksetState = SETHOUR;
          lcd.clear();
        }
        break;
      case SETHOUR:
        snprintf(buffer[0], sizeof(buffer), "SET HOUR");
        snprintf(buffer[1], sizeof(buffer), "%02d:%02d", clockSet.hour, clockSet.minute);
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == UP)
        {
          clockSet.hour++;
          if (clockSet.hour > 23)
            clockSet.hour = 0;
        }
        else if (buttonState == DOWN)
        {
          clockSet.hour--;
          if (clockSet.hour > 23)
            clockSet.hour = 23;
        }
        else if (buttonState == NEXT)
        {
          clocksetState = SETMIN;
          lcd.clear();

        }
        break;
      case SETMIN:
        snprintf(buffer[0], sizeof(buffer), "SET MIN");
        snprintf(buffer[1], sizeof(buffer), "%02d:%02d", clockSet.hour, clockSet.minute);
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        lcd.setCursor(0, 1);
        lcd.print(buffer[1]);
        if (buttonState == UP)
        {
          clockSet.minute++;
          if (clockSet.minute > 59)
            clockSet.minute = 0;
        }
        else if (buttonState == DOWN)
        {
          clockSet.minute--;
          if (clockSet.minute > 59)
            clockSet.minute = 59;
        }
        else if (buttonState == NEXT)
        {
          clocksetState = EXIT;
          lcd.clear();

        }
        break;
      case EXIT:
        sprintf(buffer[0], "EXIT");
        lcd.setCursor(0, 0);
        lcd.print(buffer[0]);
        if (buttonState == NEXT)
        {
          clocksetState = SETDAY;
          lcd.clear();
        }
        if (buttonState == SET)
        {
          rtc.setTime(clockSet.hour, clockSet.minute, 0);
          rtc.setDOW(clockSet.DOW);
          lcd.clear();
          return;
        }
        break;
    }
  }
}

button_state HandleButton()
{
  if (MenuButton.read())
    return SET;
  else if (NextButton.read())
    return NEXT;
  else if (UpButton.read())
    return UP;
  else if (DownButton.read())
    return DOWN;
  else
    return NONE;
}

#endif
