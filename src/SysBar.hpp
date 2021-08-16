#pragma once 

#include <lvgl.h>
#include "LvStyle.hpp"
#include "LvObj.hpp"

class SysBar: public LvObj {
protected:
  LvStyle m_Style;
public:
  SysBar() : LvObj(LvObj::layerTop()), m_Style() {
    
  }
};