
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

Adafruit_SH1106 g_display(-1);

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

#include "Button.h"
#include "Constants.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Utils.h"

// ------------------------------------------

enum class GameState : uint8_t
{
    Init,
    Game,
    End
};

GameState g_gameState = GameState::Init;
bool g_hasEnteredState = true;


Player g_player(0, 58, 4, 50);
Object g_playerBullet(0, 0, 2, 50);

constexpr int ENEMIES_SIZE = 6;
Enemy g_enemies[ENEMIES_SIZE];
Object g_enemyBullets[ENEMIES_SIZE];

unsigned long g_prevTimeMs = 0;
float g_timeToNextLevelS = TIME_OF_ONE_LEVEL_s;
unsigned g_score = 0;
unsigned g_levelMultiplayer = 0;

// ------------------------------------------

void setup()
{                
    initButton(BUTTON_LEFT);
    initButton(BUTTON_RIGHT);
    initButton(BUTTON_FIRE);

    g_display.begin(SH1106_SWITCHCAPVCC, 0x3C);

    g_display.clearDisplay();
}

void loop()
{
    if (g_hasEnteredState)
    {
        g_hasEnteredState = false;

        switch (g_gameState)
        {
        case GameState::Init:
            enterInit();
            break;
        case GameState::Game:
            enterGame();
            break;
        case GameState::End:
            enterEnd();
            break;
        }
    }

    switch (g_gameState)
    {
    case GameState::Init:
    {
        if (isButtonDown(BUTTON_FIRE))
            nextState();
        break;
    }
    case GameState::Game:
    {
        unsigned long currentTimeMs = millis();
        float elapsedS = ((float)(currentTimeMs - g_prevTimeMs)) / 1000.f;

        updateGame(elapsedS);

        if (g_player.m_lives <= 0)
            nextState();

        g_display.clearDisplay();
        renderGame();
        g_display.display();

        g_prevTimeMs = currentTimeMs;

        break;
    }
    case GameState::End:
    {
        if (isButtonDown(BUTTON_FIRE))
            nextState();
        break;
    }
    }
}

// ------------------------------------------

void nextState()
{
    g_hasEnteredState = true;
    switch (g_gameState)
    {
    case GameState::Init:
        g_gameState = GameState::Game;
        break;
    case GameState::Game:
        g_gameState = GameState::End;
        break;
    case GameState::End:
        g_gameState = GameState::Game;
        break;
    }
}

void enterInit()
{
    g_display.clearDisplay();
    g_display.setTextSize(1);
    g_display.setTextColor(WHITE);
    g_display.setCursor(47, 0);
    g_display.println(F("Hello! \n To start press Fire"));
    g_display.display();
}

void enterEnd()
{
    g_display.clearDisplay();
    g_display.setTextSize(1);
    g_display.setTextColor(WHITE);
    g_display.setCursor(35, 0);
    g_display.println(F("Your score:"));
    g_display.setCursor(55, 25);
    g_display.println(g_score);
    g_display.display();
}

void enterGame()
{
    g_prevTimeMs = millis();
    randomSeed(g_prevTimeMs);

    g_player.m_isActive = true;
    g_player.m_posX = 0;
    g_player.m_lives = MAX_LIVES;

    for (int i = 0; i < ENEMIES_SIZE; ++i)
    {
        g_enemies[i].reset();
        g_enemies[i].m_posY = i * g_enemies[i].m_radius + ENEMY_OFFSET_FROM_TOP;

        g_enemyBullets[i].m_speed = -ENEMY_BULLET_SPEED;
        g_enemyBullets[i].m_isActive = false;
    }
    g_score = 0;
    g_timeToNextLevelS = 0.f;
    g_levelMultiplayer = 0;
}

// ------------------------------------------

void updateGame(float _dt)
{
    g_timeToNextLevelS -= _dt;
    if (g_timeToNextLevelS <= 0.f)
    {
        g_levelMultiplayer++;
        g_timeToNextLevelS = TIME_OF_ONE_LEVEL_s;
    }

    g_player.update( _dt, isButtonDown(BUTTON_LEFT), isButtonDown(BUTTON_RIGHT));
    updateBullet(g_playerBullet, g_player, _dt, isButtonDown(BUTTON_FIRE));
    
    for (int i = 0; i < ENEMIES_SIZE; ++i)
    {
        g_enemies[i].update(_dt);
        updateBullet(g_enemyBullets[i], g_enemies[i], _dt, true);

        if (g_playerBullet.m_isActive && circleVsCircle(g_enemyBullets[i], g_playerBullet))
        {
            g_enemyBullets[i].m_isActive = false;
            g_enemyBullets[i].m_speed = -(ENEMY_BULLET_SPEED + g_levelMultiplayer);
            g_playerBullet.m_isActive = false;
        }

        if (g_playerBullet.m_isActive && circleVsCircle(g_enemies[i], g_playerBullet))
        {
            g_playerBullet.m_isActive = false;
            g_enemies[i].reset();

            g_score++;
        }

        if (circleVsCircle(g_enemyBullets[i], g_player))
        {
            g_enemyBullets[i].m_isActive = false;
            g_enemyBullets[i].m_speed = -(ENEMY_BULLET_SPEED + g_levelMultiplayer);
            
            g_player.m_isActive = false;
            g_player.m_lives--;
        }
    }
}

void renderGame()
{
    drawPlayer(g_display, g_player);
    if(g_playerBullet.m_isActive)
        drawBullet(g_display, g_playerBullet);

    for (int i = 0; i < ENEMIES_SIZE; ++i)
    {
        if(g_enemyBullets[i].m_isActive)
            drawBullet(g_display, g_enemyBullets[i]);
        drawEnemy(g_display, g_enemies[i]);
    }
}

// ------------------------------------------

void drawBullet(Adafruit_SH1106& _display, const Object& _object)
{
    _display.fillCircle(_object.m_posX, _object.m_posY, _object.m_radius, WHITE);
}

void drawEnemy(Adafruit_SH1106& _display, const Enemy& _enemy)
{
    _display.fillTriangle(
        _enemy.m_posX, _enemy.m_posY - _enemy.m_radius,
        _enemy.m_posX - _enemy.m_radius, _enemy.m_posY + _enemy.m_radius,
        _enemy.m_posX + _enemy.m_radius, _enemy.m_posY + _enemy.m_radius,
        WHITE);
}

void drawPlayer(Adafruit_SH1106& _display, const Player& _player)
{
    _display.drawBitmap(
        _player.m_posX - _player.m_radius,
        _player.m_posY - _player.m_radius,
        g_bitmapAllPlayerSprites[_player.m_animation], PLAYER_SPRITE_SIZE, PLAYER_SPRITE_SIZE, 1);
}
