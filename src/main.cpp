#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"
#include "LvAnim.hpp"
#include "LvScreen.hpp"
#include "LvLabel.hpp"
#include "LvLine.hpp"
#include "LvStyle.hpp"
#include "LvStyleTransition.hpp"
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
static LvStyle styleBackUser1;
static LvStyle styleTitle;
static LvStyle styleAlpha8;
static SysBar* sysbar;
static LvTimer* rollingTimer;
static LvStyleTransition<LV_STYLE_BG_COLOR> transitionBg(LvAnim::pathLinear, 1000);

void addState1(lv_timer_t* t);
void clearState(lv_timer_t* t) {
  mainScreen->clearState(LV_STATE_USER_1 | LV_STATE_USER_2 | LV_STATE_USER_3);
  rollingTimer->callback(addState1);
}
void addState3(lv_timer_t* t) { 
  mainScreen->addState(LV_STATE_USER_3);
  rollingTimer->callback(clearState);
}
void addState2(lv_timer_t* t) { 
  mainScreen->addState(LV_STATE_USER_2);
  rollingTimer->callback(addState3);
}
void addState1(lv_timer_t* t) { 
  mainScreen->addState(LV_STATE_USER_1);
  rollingTimer->callback(addState2);
}

void build_ui(void) {
  lv_theme_default_init(
    NULL, 
    lv_color_white(), 
    lv_palette_main(LV_PALETTE_DEEP_ORANGE), 
    LV_THEME_DEFAULT_DARK, 
    &lv_font_montserrat_14
  );

  styleBack.bgColor(WAVE_GRADIENT_START);
  styleBack.transition(transitionBg);

  styleTitle.textFont(&lv_font_montserrat_26);
  styleTitle.textColor(lv_theme_get_color_primary(NULL));
  styleAlpha8.textFont(&lv_font_montserrat_26);
  styleAlpha8.imgRecolorOpa(LV_OPA_COVER);
  styleAlpha8.imgRecolor(lv_theme_get_color_primary(NULL));

  mainScreen = new WaveScreen<WAVE_COUNT, WAVE_POINT_COUNT>(WAVE_FPS);
  mainScreen->addStyle(styleBack);

  mainScreen->styleTransition(transitionBg, LV_STATE_USER_1);
  mainScreen->styleTransition(transitionBg, LV_STATE_USER_2);
  mainScreen->styleTransition(transitionBg, LV_STATE_USER_3);
  mainScreen->styleBgColor(lv_palette_main(LV_PALETTE_PURPLE), LV_STATE_USER_1);
  mainScreen->styleBgColor(lv_palette_main(LV_PALETTE_ORANGE), LV_STATE_USER_2);
  mainScreen->styleBgColor(lv_palette_main(LV_PALETTE_BLUE), LV_STATE_USER_3);
  mainScreen->load();
  mainScreen->start();

  sysbar = new SysBar();

  rollingTimer = new LvTimer(addState1, 2000, LvTimer::RepeatInfinite);
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
