#pragma once

#include "Constants.h"
#include "Object.h"

class Enemy : public Object
{
public:
    Enemy() = default;
    Enemy(float _posX, float _posY, uint8_t _radius, uint8_t _speed);

    void update(float _dt);
    void reset();

private:
    int8_t m_direction = 1;
};

Enemy::Enemy(float _posX, float _posY, uint8_t _radius, uint8_t _speed)
    : Object(_posX, _posY, _radius, _speed)
{
}

void Enemy::update(float _dt)
{
    if (m_posX <= 0)
        m_direction = 1;
    else if (m_posX >= DISPLAY_WIDTH)
        m_direction = -1;
    m_posX += m_direction * m_speed * _dt;   
}

void Enemy::reset()
{
    m_radius = random(2, 4);
    m_speed = random(10, 25);
    uint8_t randPos = random(DISPLAY_WIDTH);
    m_direction = (randPos % 2) ? 1 : -1;
    m_posX = randPos;
    m_isActive = true;
}
