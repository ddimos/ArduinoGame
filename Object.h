#pragma once

#include "Constants.h"

class Object
{
public:
    Object() = default;
    Object(float _posX, float _posY, uint8_t _radius, uint8_t _speed);

    float m_posX = 0;
    float m_posY = 0;
    uint8_t m_radius = 0;
    int8_t m_speed = 0;
    bool m_isActive = false;
};

Object::Object(float _posX, float _posY, uint8_t _radius, uint8_t _speed)
    : m_posX{_posX}, m_posY{_posY}, m_radius{_radius}, m_speed{_speed}
{
}

bool updateBullet(Object& _bullet, Object& _owner, float _dt, bool _fire)
{
    if(_bullet.m_isActive)
    {
        _bullet.m_posY -= _bullet.m_speed * _dt;
        if (_bullet.m_posY < 0 || _bullet.m_posY > DISPLAY_HEIGHT)
            _bullet.m_isActive = false;

        return;
    }

    if (!_fire || !_owner.m_isActive)
        return;

    _bullet.m_isActive = true;
    _bullet.m_posX = _owner.m_posX;
    _bullet.m_posY = _owner.m_posY;
}
