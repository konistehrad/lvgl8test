#pragma once

#include <lvgl.h>
#include "LvObj.hpp"

template<int NPoints>
class LvLine: public LvObj {
private:
  lv_point_t m_Points[NPoints];
protected:
  void dirty() { lv_line_set_points(m_RawObj, m_Points, NPoints); }
public:
  LvLine(): LvObj(&lv_line_create) {
    lv_line_set_points(m_RawObj, m_Points, NPoints);
  }
  LvLine(lv_obj_t* parent): LvObj(parent, &lv_line_create) {
    lv_line_set_points(m_RawObj, m_Points, NPoints);
  }
  void yInvert(bool inv) { lv_line_set_y_invert(m_RawObj, inv); }
  bool yInvert() { return lv_line_get_y_invert(m_RawObj); }
  constexpr uint16_t size() const { return NPoints; }
  lv_point_t& operator[](int idx) const { return m_Points[idx]; } 
  lv_point_t& operator[](int idx) { dirty(); return m_Points[idx]; } 
};
