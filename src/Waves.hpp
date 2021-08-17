#pragma once 

#include <math.h>
#include <lvgl.h>
#include "LvStyle.hpp"
#include "LvLine.hpp"

#define WAVE_WATER_HEIGHT (LV_VER_RES/6)
#define WAVE_END_POS (LV_HOR_RES)
#define WAVE_PALETTE (LV_PALETTE_TEAL)
#define WAVE_GRADIENT_START (lv_palette_darken(WAVE_PALETTE, 4))
#define WAVE_GRADIENT_END (lv_palette_darken(WAVE_PALETTE, 3))
#define WAVE_COLOR (lv_palette_darken(WAVE_PALETTE, 3))
#define WAVE_OPACITY (LV_OPA_100)

static uint32_t g_seed = 2378462;
inline int fast_rand(void) { g_seed = (214013*g_seed+2531011); return (g_seed>>16)&0x7FFF; }
inline int random(int max, int min = 0) { return (fast_rand() % (max-min)) + min; }

// lvgl reimplementation of: https://board.flashkit.com/board/showthread.php?798917-psp-xmb-menu&p=4188350&viewfull=1#post4188350
template <int NPoints>
class Wave: public LvLine<NPoints>
{
private:
  static constexpr float pi = 3.14159265358979323846f;
  static constexpr float pi2 = pi * 2;
  static constexpr float r(int i) { return (float)i / (NPoints-1); }
  static constexpr float r2pi(int i) { return r(i) * r(i) * pi2; }
public: 
  static constexpr int size() { return NPoints; }

public:
  Wave(lv_obj_t* parent, int idx) : LvLine<NPoints>(parent), m_Index(idx) { 
    m_LH = random(30);
    m_SY = m_LH + WAVE_WATER_HEIGHT;
    m_Speed = -(0.f + (((float)random(20,3) / 2.f) + .1f) / 10000.f);

    m_Style.lineColor(WAVE_COLOR);
    m_Style.lineWidth(m_Index % 3 == 0 ? 2 : 1);
    m_Style.lineOpa(WAVE_OPACITY);
    LvLine<NPoints>::addStyle(m_Style);

    for(int i = 0; i < NPoints; ++i) {
      LvLine<NPoints>::m_Points[i].x = r(i) * WAVE_END_POS;
    }
    LvLine<NPoints>::dirty();
    LvLine<NPoints>::center();
  }
  void tick() {
    float speedFactor = getTimer() * m_Speed;
    for(int i = 0; i < NPoints; ++i) {
      LvLine<NPoints>::m_Points[i].y = m_SY + WAVE_WATER_HEIGHT * cosf(r2pi(i) + speedFactor);
    }
    LvLine<NPoints>::dirty();
  }
private:
  inline float getTimer() { return lv_tick_get()+10000; }
  float m_PX;
  float m_PY;
  uint32_t m_LH;
  uint32_t m_SY;
  uint32_t m_Index;
  float m_Speed;
  LvStyle m_Style;
};

template <int NWaves, int NPoints>
class WaveScreen
{
private:
  static void Tick(lv_timer_t* timer) {
    WaveScreen* sc = (WaveScreen*)timer->user_data;
    Wave<NPoints>** waves = sc->m_Waves;
    for(int i = 0; i < NWaves; ++i) waves[i]->tick();
  }
public:
  static constexpr int size() { return NWaves; }
public:
  WaveScreen(lv_obj_t* parent, int fps = 30) 
    : m_Spf(1000.f / (float)fps),
      m_Timer(NULL) {
    for(int i = 0; i < NWaves; ++i) {
      m_Waves[i] = new Wave<NPoints>(parent, i);
      m_Waves[i]->tick();
    }
  }
  ~WaveScreen() {
    stop();
    for(int i = 0; i < NWaves; ++i) delete m_Waves[i];
  }
  void start() {
    if(m_Timer != NULL) return;
    m_Timer = lv_timer_create(WaveScreen::Tick, m_Spf, (void*)this);
    lv_timer_set_repeat_count(m_Timer, -1);
  }
  void stop() {
    if(m_Timer == NULL) return;
    lv_timer_del(m_Timer);
    m_Timer = NULL;
  }

private:
  lv_timer_t* m_Timer;
  int m_Spf;
  Wave<NPoints>* m_Waves[NWaves];
};
