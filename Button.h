#pragma once

inline void initButton(int _pin)
{
    pinMode(_pin, INPUT);
}

inline bool isButtonDown(int _pin)
{
    return digitalRead(_pin) == HIGH;
}
