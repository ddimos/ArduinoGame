#pragma once

#include "Object.h"

bool circleVsCircle(Object& _obj1, Object& _obj2)
{
    float fromAtoBX = _obj1.m_posX - _obj2.m_posX;
    float fromAtoBY = _obj1.m_posY - _obj2.m_posY;
    
    int rSqr = _obj1.m_radius + _obj2.m_radius + 1;
    rSqr *= rSqr;
    float distSqr = fromAtoBX * fromAtoBX + fromAtoBY * fromAtoBY;
    
    if (rSqr <= distSqr)
        return false;
    return true;
}
