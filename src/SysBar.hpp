#pragma once 

#include <lvgl.h>
#include "LvObj.hpp"

class SysBar: public LvObj {
public:
  SysBar() : LvObj(LvObj::layerTop()) { }
};