#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h> 
#include "app_hal.h"
#include "esp_freertos_hooks.h"

TFT_eSPI tft = TFT_eSPI();
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

static void lv_tick_task(void)
{
 lv_tick_inc(portTICK_RATE_MS);
}

void hal_setup() {
  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf[LV_HOR_RES * 10];
  static lv_disp_drv_t disp_drv;
  // static lv_indev_drv_t indev_drv;

  esp_err_t err = esp_register_freertos_tick_hook((esp_freertos_tick_cb_t)lv_tick_task); 

  tft.begin();
  tft.setRotation(1);

  // Enable Backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL,1);

  lv_disp_draw_buf_init(&disp_buf, buf, NULL, LV_HOR_RES * 10);
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 480;
  disp_drv.ver_res = 320;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv);
}

void hal_loop() {
  lv_task_handler();
  delay(1);
}
