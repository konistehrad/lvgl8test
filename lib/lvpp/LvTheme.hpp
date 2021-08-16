#pragma once

#include <lvgl.h>

class LvTheme {
private:
  static LvTheme& defaultTheme;
public:
  static void defaultInit(lv_disp_t* disp, lv_color_t color_primary, lv_color_t color_secondary, bool dark, const lv_font_t * font) {
    if(defaultInited()) return;
    defaultTheme = LvTheme(lv_theme_default_init(disp, color_primary, color_secondary, dark, font));
  }
  static bool defaultInited() { return lv_theme_default_is_inited(); }
private:
  lv_theme_t* m_RawTheme;
public:
  LvTheme(lv_theme_t* raw) : m_RawTheme(raw) {}
};
