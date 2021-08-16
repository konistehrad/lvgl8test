#pragma once

#include <iostream>
#include "LvObj.hpp"

#define LvPPLabelProxySimple(lvpropname, lvproptype) \
  lvproptype lvpropname() { return lv_label_get_##lvpropname(m_RawObj); } \
  void lvpropname(lvproptype val) { lv_label_set_##lvpropname(m_RawObj, val); }

#define LvPPLabelProxy(lvcamelname, lvpropname, lvproptype) \
  lvproptype lvcamelname() { return lv_label_get_##lvpropname(m_RawObj); } \
  void lvcamelname(lvproptype val) { lv_label_set_##lvpropname(m_RawObj, val); }

class LvLabel: public LvObj {
protected:
public:
  LvLabel(): LvObj(&lv_label_create) {}
  LvLabel(const char* txt): LvObj(&lv_label_create) {text(txt, false);}
  LvLabel(lv_obj_t* parent) : LvObj(parent, &lv_label_create) {}
  LvLabel(lv_obj_t* parent, const char* txt) : LvObj(parent, &lv_label_create) {text(txt, false);}
  LvPPLabelProxy(longMode, long_mode, lv_label_long_mode_t);
  LvPPLabelProxySimple(recolor, bool);
  char* text() { return lv_label_get_text(m_RawObj); }
  void text(const char* text, bool staticText = false) { 
    if(staticText) {
      lv_label_set_text_static(m_RawObj, text); 
    } else {
      lv_label_set_text(m_RawObj, text); 
    }
  }
};
