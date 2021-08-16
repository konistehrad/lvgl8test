#pragma once

#include <lvgl.h>
#include "LvObj.hpp"

#define LvPPImgProxy(lvcamelname, lvpropname, lvproptype) \
  lvproptype lvcamelname() { return lv_img_get_##lvpropname(m_RawObj); } \
  void lvcamelname(lvproptype val) { lv_img_set_##lvpropname(m_RawObj, val); }

#define LvPPImgProxySimple(lvpropname, lvproptype) \
  lvproptype lvpropname() { return lv_img_get_##lvpropname(m_RawObj); } \
  void lvpropname(lvproptype val) { lv_img_set_##lvpropname(m_RawObj, val); }

class LvImage: public LvObj {
public:
	LvImage(lv_obj_t* parent, lv_img_dsc_t* descriptor): LvObj(parent, &lv_img_create){
    lv_img_set_src(m_RawObj, descriptor);
  }
	LvImage(lv_img_dsc_t* descriptor): LvObj(&lv_img_create) {
    lv_img_set_src(m_RawObj, descriptor);
  }
  LvPPImgProxySimple(angle, int32_t);
  LvPPImgProxySimple(antialias, bool);
  LvPPImgProxy(offsetX, offset_x, lv_coord_t);
  LvPPImgProxy(offsetY, offset_y, lv_coord_t);
  void pivot(lv_coord_t x, lv_coord_t y) { lv_img_set_pivot(m_RawObj, x, y); }
  void pivot(lv_point_t& p) { pivot(p.x, p.y); }
  lv_point_t pivot() { 
    lv_point_t result;
    lv_img_get_pivot(m_RawObj, &result);
    return result;
  }
  LvPPImgProxySimple(src, const void*);
  LvPPImgProxySimple(zoom, uint16_t);
};
