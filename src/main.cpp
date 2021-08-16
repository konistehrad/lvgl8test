#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"
#include "LvLabel.hpp"
#include "LvLine.hpp"
#include "LvStyle.hpp"
#include "LvTheme.hpp"
#include "Waves.hpp"

static WaveScreen mainScreen;
static LvStyle styleBack;
static LvStyle styleTitle;
static LvStyle styleAlpha8;
static LvLabel* label;

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
  lv_obj_add_style(lv_scr_act(), styleBack, 0);
  mainScreen.init(lv_scr_act());

  label = new LvLabel();
  label->text("This is a test.");
  label->addStyle(styleTitle);
  label->center();
}
extern "C" {
  void setup();
  void loop();
}

void setup() {
  lv_init();

  hal_setup();

  build_ui();
}

void loop() {
  hal_loop();
}
