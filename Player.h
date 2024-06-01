#pragma once

#include "Object.h"

enum PlayerAnimation : uint8_t
{
    Death_0 = 0,
    Death_1 = 1,
    Normal = 2,
    Right = 3,
    Left = 4,
};

class Player : public Object
{
public:
    Player(float _posX, float _posY, uint8_t _radius, uint8_t _speed);

    void update(float _dt, bool _left, bool _right);

    PlayerAnimation m_animation;
    uint8_t m_lives = MAX_LIVES;

private:
    PlayerAnimation getNextAnimation(bool _left, bool _right) const;
    float getNextAnimationTime() const;

    float m_animationTimeS = 0;
};

Player::Player(float _posX, float _posY, uint8_t _radius, uint8_t _speed)
    : Object(_posX, _posY, _radius, _speed)
{
}

void Player::update(float _dt, bool _left, bool _right)
{
    if (m_isActive)
    {
        int8_t deltaPosX = 0;
        if (_left)
            deltaPosX -= 1;
        if (_right)
            deltaPosX += 1;

        m_posX += deltaPosX * m_speed * _dt;
    }

    if (m_animationTimeS > 0.f)
    {
        m_animationTimeS -= _dt;
    }
    else
    {
        if (m_animation == PlayerAnimation::Death_0)
        {
            m_isActive = true;
            m_posX = 0;
        }
        m_animation = getNextAnimation(_left, _right);
        m_animationTimeS = getNextAnimationTime();
    }
}

PlayerAnimation Player::getNextAnimation(bool _left, bool _right) const
{
    if(m_isActive)
    {
        if (_left && !_right)
            return PlayerAnimation::Left;
        if (!_left && _right)
            return PlayerAnimation::Right;
        return PlayerAnimation::Normal;
    }

    if (m_animation >= PlayerAnimation::Normal)
        return PlayerAnimation::Death_1;
    return PlayerAnimation::Death_0;
}

float Player::getNextAnimationTime() const
{
    if (m_animation >= PlayerAnimation::Normal)
        return 0.f;
    return TIME_BETWEEN_ANIMATIONS_s;
}

// All sprites are 10x10px.
// Generated using image2cpp 
const unsigned char pm_bitmapPlayerSpriteDeath_0 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};
const unsigned char pm_bitmapPlayerSpriteDeath_1 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x1e, 0x00, 
    0x00, 0x00, 0x00, 0x00
};
const unsigned char pm_bitmapPlayerSpriteNormal [] PROGMEM = {
    0x1e, 0x00, 0x7f, 0x80, 0x7f, 0x80, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0x7f, 0x80, 
    0x7f, 0x80, 0x1e, 0x00
};
const unsigned char pm_bitmapPlayerSpriteRight [] PROGMEM = {
    0x00, 0x00, 0x0f, 0x00, 0x1f, 0x80, 0x3f, 0xc0, 0x3f, 0xc0, 0x7f, 0xc0, 0x7f, 0xc0, 0xff, 0xc0, 
    0xff, 0x80, 0xfe, 0x00
};
const unsigned char pm_bitmapPlayerSpriteLeft [] PROGMEM = {
    0x00, 0x00, 0x3c, 0x00, 0x7e, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x80, 0xff, 0x80, 0xff, 0xc0, 
    0x7f, 0xc0, 0x1f, 0xc0
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 240)
const unsigned char* g_bitmapAllPlayerSprites[5] =
{
    pm_bitmapPlayerSpriteDeath_0,
    pm_bitmapPlayerSpriteDeath_1,
    pm_bitmapPlayerSpriteNormal,
    pm_bitmapPlayerSpriteRight,
    pm_bitmapPlayerSpriteLeft
};
