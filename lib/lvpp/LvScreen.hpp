#pragma once

#include "LvObj.hpp"

class LvScreen: public LvObj {
public:
  static LvScreen& active() { 
    static LvScreen* scr;
    if(scr == NULL) scr = new LvScreen(lv_scr_act());
    scr->m_RawObj = lv_scr_act();
    return *scr;
  }
protected:
  LvScreen(lv_obj_t* active): LvObj(active, NULL) {}
public:
  LvScreen(): LvObj(NULL, lv_obj_create) {}
  void load() { lv_scr_load(m_RawObj); }
  void loadAnim(lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay = 0, bool auto_del = true) {
    lv_scr_load_anim(m_RawObj, anim_type, time, delay, auto_del);
  }
};
