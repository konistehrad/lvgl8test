#include <math.h>
#include <stdio.h>
#include <lvgl.h>
#include "app_hal.h"

#define WAVE_FPS 30
#define WAVE_COUNT 3
#define WAVE_POINT_COUNT 20
#define WAVE_WATER_LEVEL (0)
#define WAVE_WATER_HEIGHT (LV_VER_RES/4)
#define WAVE_START_POS 0
#define WAVE_END_POS (LV_HOR_RES)
#define WAVE_PALETTE (LV_PALETTE_TEAL)
#define WAVE_GRADIENT_START (lv_palette_darken(WAVE_PALETTE, 4))
#define WAVE_GRADIENT_END (lv_palette_darken(WAVE_PALETTE, 3))
#define WAVE_COLOR (lv_palette_darken(WAVE_PALETTE, 3))
#define WAVE_OPACITY (LV_OPA_100)
#define PI2 (M_PI * 2)

static uint32_t g_seed = 2378462;
inline int fast_rand(void) { g_seed = (214013*g_seed+2531011); return (g_seed>>16)&0x7FFF; }
inline int random(int max, int min = 0) { return (fast_rand() % (max-min)) + min; }

// lvgl reimplementation of: https://board.flashkit.com/board/showthread.php?798917-psp-xmb-menu&p=4188350&viewfull=1#post4188350
class Wave
{
public:
  Wave() : m_Index(++accumulator), m_Line(NULL) { }
  ~Wave() {
    if(m_Line != NULL) lv_obj_del(m_Line);
  }
  void init(lv_obj_t* parent) {
    m_LH = random(30);
    m_SY = m_LH + WAVE_WATER_HEIGHT + WAVE_WATER_LEVEL;
    m_Speed = -(0.f + (((float)random(15,3) / 2.f) + .1f) / 10000.f);

    lv_style_init(&m_LineStyle);
    lv_style_set_line_width(&m_LineStyle, m_Index % 3 == 0 ? 2 : 1);
    lv_style_set_line_color(&m_LineStyle, WAVE_COLOR);
    lv_style_set_line_opa(&m_LineStyle, WAVE_OPACITY);

    m_Line = lv_line_create(parent);
    lv_line_set_points(m_Line, points, WAVE_POINT_COUNT);
    lv_obj_add_style(m_Line, &m_LineStyle, 0);
    lv_obj_center(m_Line);
  }
  void tick() {
    for(int x = 0; x < WAVE_POINT_COUNT; ++x) {
      float r = (float)x / (WAVE_POINT_COUNT - 1);
      float px = WAVE_START_POS + r * (WAVE_END_POS - WAVE_START_POS);
      float py = m_SY + WAVE_WATER_HEIGHT * sin(r * r * PI2 + getTimer() * m_Speed);
      points[x].x = px;
      points[x].y = py;
    }
    lv_line_set_points(m_Line, points, WAVE_POINT_COUNT);
  }
private:
  inline float getTimer() { return lv_tick_get()+10000; }
  float m_PX;
  float m_PY;
  uint32_t m_LH;
  uint32_t m_SY;
  uint32_t m_Index;
  float m_Speed;
  lv_obj_t* m_Line;
  lv_point_t points[WAVE_POINT_COUNT];
  lv_style_t m_LineStyle;
  static uint32_t accumulator;
};
uint32_t Wave::accumulator = 0;

#define WAVE_MSPF (1000/WAVE_FPS)
class WaveScreen
{
public:
  WaveScreen() : m_Timer(NULL), m_Obj(NULL) {}
  void init(lv_obj_t* parent) {
    for(int i = 0; i < WAVE_COUNT; ++i)
      m_Waves[i].init(parent);
    
    m_Timer = lv_timer_create(WaveScreen::tickTrampoline, WAVE_MSPF, static_cast<void*>(this));
    lv_timer_set_repeat_count(m_Timer, -1);
  }
  ~WaveScreen() {
    if(m_Timer != NULL) lv_timer_del(m_Timer);
    if(m_Obj != NULL) lv_obj_del(m_Obj);
  }

private:
  static void tickTrampoline(lv_timer_t * timer) {
    static_cast<WaveScreen*>(timer->user_data)->tick();
  }
  void tick() {
    for(int i = 0; i < WAVE_COUNT; ++i)
      m_Waves[i].tick();
  }
  lv_timer_t* m_Timer;
  lv_obj_t* m_Obj;
  Wave m_Waves[WAVE_COUNT];
};

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

void setup() {
  lv_init();

  hal_setup();

  build_ui();
}

void loop() {
  hal_loop();
}
