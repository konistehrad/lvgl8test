#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> 
#include "app_hal.h"

#define SCANLINES_PER_BUFFER (10)
#define BUFFER_TOTAL_SIZE (LV_HOR_RES * SCANLINES_PER_BUFFER)


static LGFX lcd; 
static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  lcd.startWrite();
  lcd.pushImageDMA(area->x1, area->y2, w, h, &color_p->full);
  lv_disp_flush_ready(disp);
}

static void TickTask(void* unused) {
  while(true) {
    delay(1);
    lv_tick_inc(1);
  }
}

// setup and loop code block
extern void _real_body();
static void ClientTask(void* unused) {
  delay(100);
  Serial.println("Client task executing!");
  _real_body();
}

TaskHandle_t tickTask = NULL;
TaskHandle_t clientTask = NULL;

static lv_color_t buf1[BUFFER_TOTAL_SIZE] __attribute__((aligned(16)));
static lv_color_t buf2[BUFFER_TOTAL_SIZE] __attribute__((aligned(16)));
void hal_setup() {
  static lv_disp_draw_buf_t disp_buf;
  static lv_disp_drv_t disp_drv;

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

  BaseType_t created = xTaskCreate(&TickTask, "Tick", 255, NULL, (configMAX_PRIORITIES - 1), &tickTask);
  if(created != pdPASS) {
    Serial.print("Failed to start tick task: "); Serial.println(created);
    while(1) {
      delay(500); digitalWrite(LED_BUILTIN, HIGH);
      delay(500); digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void _wrap_body()
{
  Serial.begin(115200);

  while(!Serial && millis() < 2000) delay(10);

  BaseType_t created = xTaskCreate(&ClientTask, "client", 20480/2, NULL, (configMAX_PRIORITIES - 2), &clientTask);
  if(created != pdPASS) {
    Serial.print("Failed to start client task: "); Serial.println(created);
    while(1) {
      delay(115); digitalWrite(LED_BUILTIN, HIGH);
      delay(115); digitalWrite(LED_BUILTIN, LOW);
    }
  }

  vTaskStartScheduler();
  while(1) ;; 
  return;
}

void hal_loop() {
  delay(5);
  lv_task_handler();
}
