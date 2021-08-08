#include <vector>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"

#define CANVAS_WIDTH LV_HOR_RES
#define CANVAS_HEIGHT LV_VER_RES
#define WAVE_COUNT 8
#define WAVE_POINT_COUNT 40
#define WAVE_WATER_LEVEL (LV_VER_RES/3)
#define WAVE_WATER_HEIGHT (LV_VER_RES/8)
#define WAVE_START_POS 0
#define WAVE_END_POS CANVAS_WIDTH
#define WAVE_PALETTE (LV_PALETTE_TEAL)
#define WAVE_GRADIENT_START (lv_palette_darken(WAVE_PALETTE, 4))
#define WAVE_GRADIENT_END (lv_palette_darken(WAVE_PALETTE, 3))
#define WAVE_COLOR (lv_palette_lighten(WAVE_PALETTE, 2))
#define WAVE_OPACITY (LV_OPA_50)
#define PI2 (M_PI * 2)

static uint32_t g_seed = 2378462;
inline int fast_rand(void) { g_seed = (214013*g_seed+2531011); return (g_seed>>16)&0x7FFF; }
inline int random(int max, int min = 0) { return (fast_rand() % (max-min)) + min; }

// lvgl reimplementation of: https://board.flashkit.com/board/showthread.php?798917-psp-xmb-menu&p=4188350&viewfull=1#post4188350
class Wave
{
public:
  Wave() : m_Index(++accumulator) {
    m_LH = random(30);
    m_SY = m_LH + WAVE_WATER_HEIGHT + WAVE_WATER_LEVEL;

    m_Speed = -(0.f + (((float)random(15,3) / 2.f) + .1f) / 10000.f);
    std::cout << "Wave #" << m_Index << " selected speed " << m_Speed << std::endl;

    lv_draw_line_dsc_init(&m_LineDesc);
    m_LineDesc.color = WAVE_COLOR;
    m_LineDesc.width = m_Index % 3 == 0 ? 2 : 1;
    m_LineDesc.opa = WAVE_OPACITY;
  }
  void tick(lv_obj_t* rawCanvas) {
    for(int x = 0; x < WAVE_POINT_COUNT; ++x) {
      float r = (float)x / (WAVE_POINT_COUNT - 1);
      float px = WAVE_START_POS + r * (WAVE_END_POS - WAVE_START_POS);
      float py = m_SY + WAVE_WATER_HEIGHT * sin(r * r * PI2 + getTimer() * m_Speed);
      points[x].x = px;
      points[x].y = py;
    }
    lv_canvas_draw_line(rawCanvas, points, WAVE_POINT_COUNT, &m_LineDesc);
  }
private:
  inline float getTimer() { return lv_tick_get()+10000; }
  float m_PX;
  float m_PY;
  uint32_t m_LH;
  uint32_t m_SY;
  uint32_t m_Index;
  float m_Speed;
  lv_draw_line_dsc_t m_LineDesc;
  lv_point_t points[WAVE_POINT_COUNT];
  static uint32_t accumulator;
};
uint32_t Wave::accumulator = 0;

#define MSPF (1000/60)
class Canvas
{
public:
  Canvas(lv_obj_t* rawCanvas) : m_Canvas(rawCanvas) {
    lv_draw_rect_dsc_init(&m_RectDsc);
    m_RectDsc.bg_grad_dir = LV_GRAD_DIR_VER;
    m_RectDsc.bg_opa = LV_OPA_COVER;
    m_RectDsc.bg_color = WAVE_GRADIENT_START;
    m_RectDsc.bg_grad_color = WAVE_GRADIENT_END;

    lv_timer_create(Canvas::tickTrampoline, MSPF, static_cast<void*>(this));
    lv_timer_set_repeat_count(m_Timer, -1);
  }

private:
  static void tickTrampoline(lv_timer_t * timer)
  {
    static_cast<Canvas*>(timer->user_data)->tick();
  }
  void tick() {
    lv_canvas_draw_rect(m_Canvas, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, &m_RectDsc);
    for(int i = 0; i < WAVE_COUNT; ++i)
      m_Waves[i].tick(m_Canvas);
  }
  lv_timer_t* m_Timer;
  lv_draw_rect_dsc_t m_RectDsc;
  lv_coord_t m_TextX;
  Wave m_Waves[WAVE_COUNT];
  lv_obj_t* m_Canvas;
};

static Canvas* smartCanvas;
static lv_obj_t* canvas;
static lv_style_t style_title;
static lv_style_t style_icon_alpha8;
void build_styles(void) {
  lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, &lv_font_montserrat_14);

  lv_style_init(&style_title);
  lv_style_set_text_font(&style_title, &lv_font_montserrat_26);
  lv_style_set_text_color(&style_title, lv_theme_get_color_primary(NULL));

  lv_style_init(&style_icon_alpha8);
  lv_style_set_text_font(&style_icon_alpha8, &lv_font_montserrat_26);
  lv_style_set_img_recolor_opa(&style_icon_alpha8, LV_OPA_COVER);
  lv_style_set_img_recolor(&style_icon_alpha8, lv_theme_get_color_primary(NULL));
}

void service_canvas() {
}

void build_ui(void) {
  LV_IMG_DECLARE(humidicon);

  static lv_color_t buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
  canvas = lv_canvas_create(lv_scr_act());
  lv_canvas_set_buffer(canvas, buffer, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
  lv_obj_center(canvas);
  smartCanvas = new Canvas(canvas);
}

int main(void) {
  g_seed = (uint32_t)rand();
	lv_init();

	hal_setup();

  build_styles();
  build_ui();

  hal_loop();
}
