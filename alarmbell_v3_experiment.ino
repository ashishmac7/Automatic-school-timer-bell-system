#include "button.h"
#include "pin.h"
#include "function.h"


Time t1;
void setup() {
  Serial.begin(115200);
  rtc.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  pinMode(MANUAL_BUTTON, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);


}
bool alarmocured = false;
long last_alarm_time;
int duration = 0;
long lastmillis3;
void loop() {
  if (alarmocured)
  {
    if (millis() - lastmillis3 <= duration*1000)
    {

      digitalWrite(RELAY, HIGH);
    }
    else
    {
     digitalWrite(RELAY, LOW);
      alarmocured=0;
      lastmillis3=millis();
    }
    






  }
  if (MenuButton.longpressed(3000))
    Menu();
  t1 = rtc.getTime();
  Serial.print("date day=");
  Serial.println(t1.min);
  if (t1.dow < 6)
  {

    for (int i = 0; i < 5; i++)
    {
      Serial.println("yes");
      int now_time = t1.hour * 60 + t1.min;
      if (now_time == timeTonum(0, i) && alarm[0][i].duration!=0)
      {
        if(now_time!=last_alarm_time)
        {
        alarmocured = true;
        lastmillis3 = millis();
        duration = alarm[0][i].duration;
        last_alarm_time=now_time;
        break;
        }
      }
    }
  }
  DisplayTime();




}
int timeTonum(int j, int i)
{
  return (alarm[j][i].hour * 60 + alarm[j][i].minute);


}
void DisplayTime()
{
  lcd.setCursor(0, 0);
  lcd.print(rtc.getDOWStr(1));
  lcd.setCursor(6, 0);
  lcd.print(rtc.getTimeStr());
}


