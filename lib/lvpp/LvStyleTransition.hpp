#pragma once

#include <vector>
#include <lvgl.h>
#include "LvAnim.hpp"

template<lv_style_prop_t... Props>
class LvStyleTransition {
public:
  static constexpr size_t size() { return sizeof...(Props) + 1; };
protected:
  lv_style_prop_t m_Props[size()] = {Props..., LV_STYLE_PROP_INV};
  lv_style_transition_dsc_t m_TransitionDsc;
public:
  LvStyleTransition(lv_anim_path_cb_t path, uint32_t timeMs, uint32_t delayMs = 0) {
    lv_style_transition_dsc_init(&m_TransitionDsc, m_Props, path, timeMs, delayMs, NULL);
  }
  operator lv_style_transition_dsc_t*() { return &m_TransitionDsc; }
};
