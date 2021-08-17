#pragma once 

#include <lvgl.h>
#include "LvStyle.hpp"
#include "LvObj.hpp"

class SysBar: public LvObj {
public:
  SysBar(lv_coord_t barHeight = 30) : LvObj(LvObj::layerTop(), &lv_obj_create) {  
    removeStyleAll();
    styleBgColor(lv_color_black(), 0);
    styleBgOpa(LV_OPA_50, 0);
    styleTextColor(lv_color_white(), 0);
    width(LV_HOR_RES);
    height(barHeight);
    align(LV_ALIGN_TOP_MID);
  }
};
