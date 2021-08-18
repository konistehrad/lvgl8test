#pragma once

#include <lvgl.h>

class LvAnim {
public:
  static uint16_t countRunnung() { return lv_anim_count_running(); }
  static void setX(void* var, int32_t v) { lv_obj_set_x((lv_obj_t*)var, v); }
  static void setY(void* var, int32_t v) { lv_obj_set_y((lv_obj_t*)var, v); }
  static void setWidth(void* var, int32_t v) { lv_obj_set_width((lv_obj_t*)var, v); }
  static void setHeight(void* var, int32_t v) { lv_obj_set_height((lv_obj_t*)var, v); }
  static void setSize(void* var, int32_t v) { lv_obj_set_size((lv_obj_t*)var, v, v); }
  
  static constexpr lv_anim_path_cb_t pathBounce = lv_anim_path_bounce;
  static constexpr lv_anim_path_cb_t pathEaseIn = lv_anim_path_ease_in;
  static constexpr lv_anim_path_cb_t pathEaseInOut = lv_anim_path_ease_in_out;
  static constexpr lv_anim_path_cb_t pathEaseOut = lv_anim_path_ease_out;
  static constexpr lv_anim_path_cb_t pathLinear = lv_anim_path_linear;
  static constexpr lv_anim_path_cb_t pathOvershoot = lv_anim_path_overshoot;
  static constexpr lv_anim_path_cb_t pathStep = lv_anim_path_step;

protected:
  lv_anim_t m_Anim;
  lv_anim_t* m_RunningAnim;
public:
  LvAnim() { lv_anim_init(&m_Anim); }
  LvAnim(lv_obj_t* subjectObj, int32_t startVal, int32_t endVal, uint32_t timeMs, lv_anim_exec_xcb_t execCb = NULL, lv_anim_path_cb_t pathCb = NULL) 
    : LvAnim() 
  { 
    subject(subjectObj);
    values(startVal, endVal);
    time(timeMs);
    if(execCb != NULL) exec(execCb);
    if(pathCb != NULL) path(pathCb);
  }
  ~LvAnim() { }
  operator lv_anim_t*() { return &m_Anim; }
  void subject(lv_obj_t* obj) { var((void*)obj); }
  void var(void* v) { lv_anim_set_var(&m_Anim, v); }
  void delay(uint32_t t) { lv_anim_set_delay(&m_Anim, t); }
  void playbackDelay(uint32_t t) { lv_anim_set_playback_delay(&m_Anim, t); }
  void repeatDelay(uint32_t t) { lv_anim_set_repeat_delay(&m_Anim, t); }
  void time(uint32_t t) { lv_anim_set_time(&m_Anim, t); }
  void path(lv_anim_path_cb_t cb) { lv_anim_set_path_cb(&m_Anim, cb); }
  void exec(lv_anim_exec_xcb_t cb) { lv_anim_set_exec_cb(&m_Anim, cb); }
  void start() { m_RunningAnim = lv_anim_start(&m_Anim); }
  void earlyApply(bool en) { lv_anim_set_early_apply(&m_Anim, en); }
  void values(int32_t startVal, int32_t endVal) { lv_anim_set_values(&m_Anim, startVal, endVal); }
};
