#pragma once 

#include <lvgl.h>
#include "LvStyle.hpp"
#include "LvObj.hpp"

class SysBar: public LvObj {
protected:
  LvStyle m_Style;
public:
  SysBar(lv_coord_t height = 50) : LvObj(LvObj::layerTop()), m_Style() {  
    m_Style.bgColor(lv_color_black());
    m_Style.bgOpa(LV_OPA_50);
    m_Style.height(height);
    m_Style.textColor(lv_color_white());
    addStyle(m_Style);
  }
};