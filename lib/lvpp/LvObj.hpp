#pragma once

#include <lvgl.h>
#include "LvStyle.hpp"

#define LvPPObjStyleProxyPart(lvcamelname, lvunderscorename, lvproptype) \
  lvproptype lvcamelname(uint32_t part) { return lv_obj_get_style_##lvunderscorename(m_RawObj, part) } \
  void lvcamelname(lvproptype val, uint32_t part) { lv_obj_set_style_##lvunderscorename(m_RawObj, val, part); }

#define LvPPObjStyleProxySelect(lvcamelname, lvunderscorename, lvproptype) \
  lvproptype lvcamelname(lv_style_selector_t selector) { return lv_obj_get_style_##lvunderscorename(m_RawObj, selector) } \
  void lvcamelname(lvproptype val, lv_style_selector_t selector) { lv_obj_set_style_##lvunderscorename(m_RawObj, val, selector); }

#define LvPPObjProxySimple(lvpropname, lvproptype) \
  lvproptype lvpropname() { return lv_obj_get_##lvpropname(m_RawObj); } \
  void lvpropname(lvproptype val) { lv_obj_set_##lvpropname(m_RawObj, val); }

class LvObj {
public:
  static LvObj& layerTop() {
    static LvObj* top;
    if(top == NULL) top = new LvObj(lv_layer_top());
    return *top;
  }
  static LvObj& layerSys() {
    static LvObj* sys;
    if(sys == NULL) sys = new LvObj(lv_layer_sys());
    return *sys;
  }
  static void initLvPP() {
    static bool inited = false;
    if(inited) return;
    lv_init();
    inited = true;
  }
protected:
  lv_obj_t* m_RawObj;
  LvObj* m_Parent;

  LvObj(lv_obj_t* rawBacking) : m_Parent(NULL), m_RawObj(rawBacking) {}
  lv_obj_t* rawParent() { return m_Parent != NULL ? m_Parent->m_RawObj : lv_scr_act(); }
  void init() { m_RawObj = createBacking(); }
  virtual lv_obj_t* createBacking() { return lv_obj_create(rawParent()); }
public:
  LvObj() : m_Parent(NULL) { init(); }
  LvObj(LvObj& parent) : m_Parent(&parent) { init(); }
  bool valid() { return m_RawObj != NULL && lv_obj_is_valid(m_RawObj); }
  operator lv_obj_t*() { return m_RawObj; }
  void addStyle(lv_style_t* style, lv_style_selector_t selector = 0) { lv_obj_add_style(m_RawObj, style, selector); }
  void removeStyle(lv_style_t* style, lv_style_selector_t selector = 0) { lv_obj_remove_style(m_RawObj, style, selector); }
  void align(lv_align_t alignment = LV_ALIGN_DEFAULT, lv_coord_t xOfs = 0, lv_coord_t yOfs = 0) { lv_obj_align(m_RawObj, alignment, xOfs, yOfs); }
  void alignTo(lv_obj_t* obj, lv_align_t alignment = LV_ALIGN_DEFAULT, lv_coord_t xOfs = 0, lv_coord_t yOfs = 0) { lv_obj_align_to(m_RawObj, obj, alignment, xOfs, yOfs); }
  void moveForeground() { lv_obj_move_foreground(m_RawObj); }
  void moveBackground() { lv_obj_move_background(m_RawObj); }
  LvPPObjProxySimple(height, lv_coord_t);
  LvPPObjProxySimple(width, lv_coord_t);
  void size(lv_coord_t w, lv_coord_t h) { lv_obj_set_size(m_RawObj, w, h); }
  LvPPObjProxySimple(x, lv_coord_t);
  LvPPObjProxySimple(y, lv_coord_t);
  void addFlag(lv_obj_flag_t flag) { lv_obj_add_flag(m_RawObj, flag); }
  void clearFlag(lv_obj_flag_t flag) { lv_obj_clear_flag(m_RawObj, flag); }
};
