#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"
#include "LvScreen.hpp"
#include "LvLabel.hpp"
#include "LvLine.hpp"
#include "LvStyle.hpp"
#include "SysBar.hpp"
#include "Waves.hpp"

#ifndef WAVE_FPS
  #define WAVE_FPS 30
#endif
#ifndef WAVE_COUNT
  #define WAVE_COUNT 6
#endif
#ifndef WAVE_POINT_COUNT
  #define WAVE_POINT_COUNT 40
#endif

static WaveScreen<WAVE_COUNT, WAVE_POINT_COUNT>* mainScreen;
static LvStyle styleBack;
static LvStyle styleTitle;
static LvStyle styleAlpha8;
static LvLabel* label;
static SysBar* sysbar;

void build_ui(void) {
  lv_theme_default_init(
    NULL, 
    lv_color_white(), 
    lv_palette_main(LV_PALETTE_DEEP_ORANGE), 
    LV_THEME_DEFAULT_DARK, 
    &lv_font_montserrat_14
  );

  styleBack.bgColor(WAVE_GRADIENT_START);
  styleTitle.textFont(&lv_font_montserrat_26);
  styleTitle.textColor(lv_theme_get_color_primary(NULL));
  styleAlpha8.textFont(&lv_font_montserrat_26);
  styleAlpha8.imgRecolorOpa(LV_OPA_COVER);
  styleAlpha8.imgRecolor(lv_theme_get_color_primary(NULL));

  mainScreen = new WaveScreen<WAVE_COUNT, WAVE_POINT_COUNT>(WAVE_FPS);
  mainScreen->addStyle(styleBack);
  mainScreen->load();

  sysbar = new SysBar();

  mainScreen->start();
}
extern "C" {
  void setup();
  void loop();
}

void setup() {
  LvObj::init();
  
  hal_setup();

  build_ui();
}

void loop() {
  hal_loop();
}
