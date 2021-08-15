#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> 
#include "app_hal.h"

#define SCANLINES_PER_BUFFER (10)
#define BUFFER_TOTAL_SIZE (LV_HOR_RES * SCANLINES_PER_BUFFER)

static LGFX lcd; 
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  lcd.pushImageDMA(area->x1, area->y2, w, h, &color_p->full);
  lv_disp_flush_ready(disp);
}

void vApplicationTickHook(void) {
 lv_tick_inc(1);
}

void hal_setup() {
  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf1[BUFFER_TOTAL_SIZE];
  static lv_color_t buf2[BUFFER_TOTAL_SIZE];
  static lv_disp_drv_t disp_drv;

  // esp_register_freertos_tick_hook((esp_freertos_tick_cb_t)lv_tick_task); 

  lcd.init();
  lcd.initDMA();
  lcd.setRotation(1);
  lcd.setColorDepth(LV_COLOR_DEPTH);
  lcd.setBrightness(128);

  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, BUFFER_TOTAL_SIZE);
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LV_HOR_RES;
  disp_drv.ver_res = LV_VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  lcd.startWrite(); // we own this SPI line.
}

void hal_loop() {
  lv_task_handler();
  delay(1);
}
