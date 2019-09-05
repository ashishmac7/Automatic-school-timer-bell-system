#ifndef button_h
#define button_h

#include<Arduino.h>

class Button
{
  public:
    boolean read();
    boolean longpressed(uint16_t debounce_dealy);

    Button(byte pin, byte state = HIGH)
    {
      _pin = pin;
      _state = state;
      pinMode(pin, INPUT_PULLUP);
    }

  private:
    boolean _last_reading = _state;
    long _timer = millis();
    boolean _buttonState = !_state;
    byte _debounceDelay = 20;
    byte _pin, _state;
};


 boolean Button::read()
{
  boolean reading = digitalRead(_pin);
  if (reading != _last_reading) {
    // reset the debouncing timer
    _timer = millis();
  }

  if ((millis() - _timer) > _debounceDelay) {
    //Serial.println("in millis");

    if (reading != _buttonState) {
      _buttonState = reading;

      if (_buttonState==_state)
      {
        _last_reading = reading;
        return true;
      }
    }
  }

  _last_reading = reading;
  return false;
}

boolean Button:: longpressed(uint16_t debounce_dealy)
{
 if(this->read())
 {
  long time = millis();
  while (digitalRead(_pin)==_state)
  {
    if (millis() - time > debounce_dealy)
    return true;
 
  }

  /*if (millis() - time > debounce_dealy)
  {
    return true;
  }*/
 }
  return false;
}
#endif
