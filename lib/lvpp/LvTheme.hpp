#pragma once

#include <lvgl.h>

class LvTheme {
private:
  lv_theme_t* m_RawTheme;
public:
  LvTheme(lv_theme_t* raw) : m_RawTheme(raw) {}
};
