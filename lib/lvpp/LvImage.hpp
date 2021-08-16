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
protected:
	lv_img_dsc_t* m_ImgDsc;
  lv_obj_t* createBacking() override { 
    lv_obj_t* img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, m_ImgDsc);
    return img;
  }
public:
	LvImage(LvObj& parent, lv_img_dsc_t* descriptor): m_ImgDsc(descriptor), LvObj(parent) {}
	LvImage(lv_img_dsc_t* descriptor): m_ImgDsc(descriptor), LvObj() {}
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

  void xxxx() {
  }
};
