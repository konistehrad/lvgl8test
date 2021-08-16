#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"
#include "LvLine.hpp"
#include "Waves.hpp"

static WaveScreen mainScreen;
static lv_style_t style_back;
static lv_style_t style_title;
static lv_style_t style_icon_alpha8;
static lv_obj_t* label;

void build_ui(void) {
  lv_theme_default_init(
    NULL, 
    lv_color_white(), 
    lv_palette_main(LV_PALETTE_DEEP_ORANGE), 
    LV_THEME_DEFAULT_DARK, 
    &lv_font_montserrat_14
  );

  lv_style_init(&style_back);
  lv_style_set_bg_color(&style_back, WAVE_GRADIENT_START);
  //lv_style_set_bg_grad_color(&style_back, WAVE_GRADIENT_END);
  //lv_style_set_bg_grad_dir(&style_back, LV_GRAD_DIR_VER);

  lv_style_init(&style_title);
  lv_style_set_text_font(&style_title, &lv_font_montserrat_26);
  lv_style_set_text_color(&style_title, lv_theme_get_color_primary(NULL));

  lv_style_init(&style_icon_alpha8);
  lv_style_set_text_font(&style_icon_alpha8, &lv_font_montserrat_26);
  lv_style_set_img_recolor_opa(&style_icon_alpha8, LV_OPA_COVER);
  lv_style_set_img_recolor(&style_icon_alpha8, lv_theme_get_color_primary(NULL));

  lv_obj_add_style(lv_scr_act(), &style_back, 0);
  mainScreen.init(lv_scr_act());

  label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "This is a test.");
  lv_obj_add_style(label, &style_title, 0);
  lv_obj_center(label);
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
