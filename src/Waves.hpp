#pragma once 

#include <math.h>
#include "LvPP.hpp"

#define WAVE_WATER_HEIGHT (LV_VER_RES/6)
#define WAVE_END_POS (LV_HOR_RES)

// lvgl reimplementation of: https://board.flashkit.com/board/showthread.php?798917-psp-xmb-menu&p=4188350&viewfull=1#post4188350
template <int NPoints>
class Wave: public LvLine<NPoints>
{
private:
  static constexpr float pi = 3.14159265358979323846f;
  static constexpr float pi2 = pi * 2;
  static constexpr float r(int i) { return (float)i / (NPoints-1); }
  static constexpr float r2pi(int i) { return r(i) * r(i) * pi2; }

  static int fast_rand(void) { 
    static uint32_t g_seed = 2378462;
    g_seed = (214013*g_seed+2531011); return (g_seed>>16)&0x7FFF; 
  }
  static int random(int max, int min = 0) { return (fast_rand() % (max-min)) + min; }
  
public: 
  static constexpr int size() { return NPoints; }

private:
  inline float getTimer() { return lv_tick_get()+10000; }
  float m_PX;
  float m_PY;
  uint32_t m_LH;
  uint32_t m_SY;
  uint32_t m_Index;
  float m_Speed;
  LvObj* m_Parent;
public:
  Wave(LvObj* parent, int idx) 
    : LvLine<NPoints>(*parent),
      m_Parent(parent),
      m_Index(idx) 
  { 
    m_LH = random(30);
    m_SY = m_LH + WAVE_WATER_HEIGHT;
    m_Speed = -(0.f + (((float)random(20,3) / 2.f) + .1f) / 10000.f);

    this->styleLineWidth(m_Index % 3 == 0 ? 2 : 1, 0);
    refreshStyles();
    this->addFlag(LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);
    for(int i = 0; i < NPoints; ++i) {
      this->m_Points[i].x = r(i) * WAVE_END_POS;
    }
    this->tick();
    this->center();
  }
  void refreshStyles() {
    this->styleLineOpa(m_Parent->styleLineOpa());
    this->styleLineColor(m_Parent->styleLineColor());
  }
  void tick() {
    float speedFactor = getTimer() * m_Speed;
    for(int i = 0; i < NPoints; ++i) {
      this->m_Points[i].y = m_SY + WAVE_WATER_HEIGHT * cosf(r2pi(i) + speedFactor);
    }
    this->dirty();
  }
};

template <int NWaves, int NPoints>
class WaveScreen: public LvScreen
{
private:
  static void StyleUpdated(lv_event_t* ev) { ((WaveScreen*)ev->user_data)->styleChanged(ev); }
  static void Tick(lv_timer_t* timer) {
    WaveScreen* sc = (WaveScreen*)timer->user_data;
    Wave<NPoints>** waves = sc->m_Waves;
    for(int i = 0; i < NWaves; ++i) waves[i]->tick();
  }
public:
  static constexpr int size() { return NWaves; }
private:
  uint32_t m_Spf;
  LvTimer m_Timer;
  Wave<NPoints>* m_Waves[NWaves];
public:
  WaveScreen(int fps = 30) 
    : LvScreen(),
      m_Spf(1000.f / (float)fps),
      m_Timer(WaveScreen<NWaves,NPoints>::Tick, m_Spf, LvTimer::RepeatInfinite, (void*)this) {
    for(int i = 0; i < NWaves; ++i)
      m_Waves[i] = new Wave<NPoints>(this, i);
    lv_obj_add_event_cb(m_RawObj, WaveScreen::StyleUpdated, LV_EVENT_STYLE_CHANGED, (void*)this);
    m_Timer.pause();
  }
  ~WaveScreen() {
    stop();
    for(int i = 0; i < NWaves; ++i) delete m_Waves[i];
    lv_obj_del(m_RawObj);
  }
  void styleChanged(lv_event_t* ev) {
    for(int i = 0; i < NWaves; ++i) m_Waves[i]->refreshStyles();
  }
  void start() { m_Timer.resume(); }
  void stop() { m_Timer.pause(); }
};
