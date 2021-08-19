#pragma once

#include <lvgl.h>

class LvTimer {
public:
  static constexpr int RepeatInfinite = -1;
  static void enable(bool en) { lv_timer_enable(en); }
  static uint8_t idle() { return lv_timer_get_idle(); }
protected:
  lv_timer_t* m_Timer;
public:
  LvTimer() { m_Timer = lv_timer_create_basic(); }
  LvTimer(lv_timer_cb_t callback, uint32_t period, int32_t repeatCount = 0, void* userData = NULL) {
    m_Timer = lv_timer_create(callback, period, userData);
    if(repeatCount != 0) lv_timer_set_repeat_count(m_Timer, repeatCount);
  }
  ~LvTimer() {
    if(m_Timer != NULL) lv_timer_del(m_Timer);
    m_Timer = NULL;
  }
  void callback(lv_timer_cb_t cb) { lv_timer_set_cb(m_Timer, cb); }
  void period(uint32_t val) { lv_timer_set_period(m_Timer, val); }
  void repeatCount(int32_t val) { lv_timer_set_repeat_count(m_Timer, val); }
  void ready() { lv_timer_ready(m_Timer); }
  void reset() { lv_timer_reset(m_Timer); }
  void resume() { lv_timer_resume(m_Timer); }
  void pause() { lv_timer_pause(m_Timer); }
};
