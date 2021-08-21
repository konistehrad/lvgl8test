#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"
#include "LvPP.hpp"
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
static LvStyle styleWaves;
static LvStyle styleTitle;
static LvStyle styleAlpha8;
static SysBar* sysbar;
static LvStyleTransition<LV_STYLE_BG_COLOR> transitionBg(LvAnim::pathLinear, 1000);

void build_ui(void) {
  lv_theme_default_init(
    NULL, 
    lv_color_white(), 
    lv_palette_darken(LV_PALETTE_TEAL, 4),
    LV_THEME_DEFAULT_DARK, 
    &lv_font_montserrat_14
  );

  styleWaves.bgColor(lv_theme_get_color_secondary(NULL));
  styleWaves.lineColor(lv_theme_get_color_primary(NULL));
  styleWaves.lineOpa(LV_OPA_10);
  
  styleTitle.textFont(&lv_font_montserrat_26);
  styleTitle.textColor(lv_theme_get_color_primary(NULL));
  styleAlpha8.textFont(&lv_font_montserrat_26);
  styleAlpha8.imgRecolorOpa(LV_OPA_COVER);
  styleAlpha8.imgRecolor(lv_theme_get_color_primary(NULL));

  mainScreen = new WaveScreen<WAVE_COUNT, WAVE_POINT_COUNT>(WAVE_FPS);
  mainScreen->addStyle(styleWaves);
  mainScreen->styleTransition(transitionBg, LV_STATE_ANY);
  mainScreen->load();
  mainScreen->start();

  sysbar = new SysBar();
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
