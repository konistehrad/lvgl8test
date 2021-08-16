#pragma once

#include <lvgl.h>
#include "LvObj.hpp"

template<int NPoints>
class LvLine: public LvObj {
private:
  lv_point_t m_Points[NPoints];
protected:
  void dirty() { lv_line_set_points(m_RawObj, m_Points, NPoints); }
  lv_obj_t* createBacking() override { 
    lv_obj_t* line = lv_line_create(rawParent()); 
    lv_line_set_points(line, m_Points, NPoints);
    return line;
  }
public:
  LvLine(): LvObj() {}
  LvLine(LvObj& parent): LvObj(parent) {}
  void yInvert(bool inv) { lv_line_set_y_invert(m_RawObj, inv); }
  bool yInvert() { return lv_line_get_y_invert(m_RawObj); }
  constexpr uint16_t size() const { return NPoints; }
  lv_point_t& operator[](int idx) const { return m_Points[idx]; } 
  lv_point_t& operator[](int idx) { dirty(); return m_Points[idx]; } 
};
