#pragma once

#include <lvgl.h>
#include "LvStyle.hpp"

#define LvPPObjStyleProxyPart(lvcamelname, lvunderscorename, lvproptype) \
  lvproptype style##lvcamelname(uint32_t part) { return lv_obj_get_style_##lvunderscorename(m_RawObj, part); } \
  void style##lvcamelname(lvproptype val, uint32_t part) { lv_obj_set_style_##lvunderscorename(m_RawObj, val, part); }

#define LvPPObjStyleProxySelect(lvcamelname, lvunderscorename, lvproptype) \
  lvproptype style##lvcamelname(lv_style_selector_t selector) { return lv_obj_get_style_##lvunderscorename(m_RawObj, selector); } \
  void style##lvcamelname(lvproptype val, lv_style_selector_t selector) { lv_obj_set_style_##lvunderscorename(m_RawObj, val, selector); }

#define LvPPObjProxy(lvcamelname, lvunderscorename, lvproptype) \
  lvproptype lvcamelname() { return lv_obj_get_##lvunderscorename(m_RawObj); } \
  void lvcamelname(lvproptype val) { lv_obj_set_##lvunderscorename(m_RawObj, val); }

#define LvPPObjProxySimple(lvpropname, lvproptype) \
  lvproptype lvpropname() { return lv_obj_get_##lvpropname(m_RawObj); } \
  void lvpropname(lvproptype val) { lv_obj_set_##lvpropname(m_RawObj, val); }

class LvObj {
public:
  static LvObj& layerTop() {
    static LvObj* top;
    if(top == NULL) top = new LvObj(lv_layer_top(), NULL);
    return *top;
  }
  static LvObj& layerSys() {
    static LvObj* sys;
    if(sys == NULL) { sys = new LvObj(lv_layer_sys(), NULL); }
    return *sys;
  }
  static void init() {
    static bool inited = false;
    if(inited) return;
    lv_init();
    inited = true;
  }
protected:
  lv_obj_t* m_RawObj;
  lv_obj_t* (*m_Factory)(lv_obj_t*);

  LvObj(lv_obj_t* rawOrParent, lv_obj_t* (*factory)(lv_obj_t*)) : m_Factory(factory) {
    m_RawObj = (factory == NULL ? rawOrParent : factory(rawOrParent));
  }
  LvObj(lv_obj_t* (*factory)(lv_obj_t*)) : LvObj(lv_scr_act(), factory) {}
public:
  LvObj() : LvObj(lv_scr_act(), &lv_obj_create) {  }
  LvObj(lv_obj_t* parent) : LvObj(parent, &lv_obj_create) { }
  ~LvObj() { 
    // check m_Factory to make sure we created the backing
    // object, if not someone else did and it's not our job.
    // (active screen?)
    if(m_RawObj != NULL && m_Factory != NULL) lv_obj_del(m_RawObj); 
    m_RawObj = NULL; 
  }
  bool valid() { return m_RawObj != NULL && lv_obj_is_valid(m_RawObj); }
  operator lv_obj_t*() { return m_RawObj; }
  void addStyle(lv_style_t* style, lv_style_selector_t selector = 0) { lv_obj_add_style(m_RawObj, style, selector); }
  void removeStyle(lv_style_t* style, lv_style_selector_t selector = 0) { lv_obj_remove_style(m_RawObj, style, selector); }
  void removeStyleAll() { lv_obj_remove_style_all(m_RawObj); }
  void align(lv_align_t alignment = LV_ALIGN_DEFAULT, lv_coord_t xOfs = 0, lv_coord_t yOfs = 0) { lv_obj_align(m_RawObj, alignment, xOfs, yOfs); }
  void alignTo(lv_obj_t* obj, lv_align_t alignment = LV_ALIGN_DEFAULT, lv_coord_t xOfs = 0, lv_coord_t yOfs = 0) { lv_obj_align_to(m_RawObj, obj, alignment, xOfs, yOfs); }
  void moveForeground() { lv_obj_move_foreground(m_RawObj); }
  void moveBackground() { lv_obj_move_background(m_RawObj); }
  LvPPObjProxySimple(height, lv_coord_t);
  LvPPObjProxySimple(width, lv_coord_t);
  void size(lv_coord_t w, lv_coord_t h) { lv_obj_set_size(m_RawObj, w, h); }
  LvPPObjProxySimple(x, lv_coord_t);
  LvPPObjProxySimple(y, lv_coord_t);
  void pos(lv_coord_t x, lv_coord_t y) { lv_obj_set_pos(m_RawObj, x, y); }
  void addFlag(lv_obj_flag_t flag) { lv_obj_add_flag(m_RawObj, flag); }
  void clearFlag(lv_obj_flag_t flag) { lv_obj_clear_flag(m_RawObj, flag); }
  void center() { lv_obj_center(m_RawObj); }
  void layout(uint32_t l) { lv_obj_set_layout(m_RawObj, l); }
  void flexAlign(lv_flex_align_t main_place, lv_flex_align_t cross_place, lv_flex_align_t track_place) {
    lv_obj_set_flex_align(m_RawObj, main_place, cross_place, track_place);
  }
  void flexFlow(lv_flex_flow_t flow) { lv_obj_set_flex_flow(m_RawObj, flow); }
  void flexGrow(uint8_t grow) { lv_obj_set_flex_grow(m_RawObj, grow); }

  LvPPObjStyleProxyPart(FlexCrossPlace, flex_cross_place, lv_flex_align_t);
  LvPPObjStyleProxyPart(FlexFlow, flex_flow, lv_flex_flow_t);
  LvPPObjStyleProxyPart(FlexGrow, flex_grow, uint8_t);
  LvPPObjStyleProxyPart(FlexMainPlace, flex_main_place, lv_flex_align_t);
  LvPPObjStyleProxyPart(FlexTrackPlace, flex_track_place, lv_flex_align_t);

  LvPPObjStyleProxySelect(Width, width, lv_coord_t);
  LvPPObjStyleProxySelect(MinWidth, min_width, lv_coord_t);
  LvPPObjStyleProxySelect(MaxWidth, max_width, lv_coord_t);

  LvPPObjStyleProxySelect(Height, height, lv_coord_t);
  LvPPObjStyleProxySelect(MinHeight, min_height, lv_coord_t);
  LvPPObjStyleProxySelect(MaxHeight, max_height, lv_coord_t);

  LvPPObjStyleProxySelect(X, x, lv_coord_t);
  LvPPObjStyleProxySelect(Y, y, lv_coord_t);
  LvPPObjStyleProxySelect(Align, align, lv_align_t);

  LvPPObjStyleProxySelect(TransformWidth, transform_width, lv_coord_t);
  LvPPObjStyleProxySelect(TransformHeight, transform_height, lv_coord_t);

  LvPPObjStyleProxySelect(TranslateX, translate_x, lv_coord_t);
  LvPPObjStyleProxySelect(TranslateY, translate_y, lv_coord_t);

  LvPPObjStyleProxySelect(TransformZoom, transform_zoom, lv_coord_t);
  LvPPObjStyleProxySelect(TransformAngle, transform_angle, lv_coord_t);

  LvPPObjStyleProxySelect(PadTop, pad_top, lv_coord_t);
  LvPPObjStyleProxySelect(PadBottom, pad_bottom, lv_coord_t);
  LvPPObjStyleProxySelect(PadLeft, pad_left, lv_coord_t);
  LvPPObjStyleProxySelect(PadRight, pad_right, lv_coord_t);
  LvPPObjStyleProxySelect(PadRow, pad_row, lv_coord_t);
  LvPPObjStyleProxySelect(PadColumn, pad_column, lv_coord_t);

  LvPPObjStyleProxySelect(Radius, radius, lv_coord_t);
  LvPPObjStyleProxySelect(ClipCorner, clip_corner, bool);

  LvPPObjStyleProxySelect(Opa, opa, lv_opa_t);
  // LvPPObjStyleProxySelect(ColorFilterDsc, color_filter_dsc, lv_color_filter_dsc_t*);
  LvPPObjStyleProxySelect(ColorFilterOpa, color_filter_opa, lv_opa_t);

  LvPPObjStyleProxySelect(AnimTime, anim_time, uint32_t);
  LvPPObjStyleProxySelect(AnimSpeed, anim_speed, uint32_t);
  // LvPPObjStyleProxySelect(Transition, transition, lv_style_transition_dsc_t*);

  LvPPObjStyleProxySelect(BlendMode, blend_mode, lv_blend_mode_t);
  LvPPObjStyleProxySelect(Layout, layout, uint16_t);
  LvPPObjStyleProxySelect(BaseDir, base_dir, lv_base_dir_t);

  LvPPObjStyleProxySelect(BgColor, bg_color, lv_color_t);
  LvPPObjStyleProxySelect(BgColorFiltered, bg_color_filtered, lv_color_t);
  LvPPObjStyleProxySelect(BgOpa, bg_opa, lv_opa_t);
  LvPPObjStyleProxySelect(BgGradColor, bg_grad_color, lv_color_t);
  LvPPObjStyleProxySelect(BgGradColorFiltered, bg_grad_color_filtered, lv_color_t);
  LvPPObjStyleProxySelect(BgGradDir, bg_grad_dir, lv_grad_dir_t);
  LvPPObjStyleProxySelect(BgImgSrc, bg_img_src, const void*);
  LvPPObjStyleProxySelect(BgImgOpa, bg_img_opa, lv_opa_t);
  LvPPObjStyleProxySelect(BgImgRecolor, bg_img_recolor, lv_color_t);
  LvPPObjStyleProxySelect(BgImgRecolorFiltered, bg_img_recolor_filtered, lv_color_t);
  LvPPObjStyleProxySelect(BgImgRecolorOpa, bg_img_recolor_opa, lv_opa_t);
  LvPPObjStyleProxySelect(BgImgTiled, bg_img_tiled, bool);

  LvPPObjStyleProxySelect(BorderColor, border_color, lv_color_t);
  LvPPObjStyleProxySelect(BorderColorFiltered, border_color_filtered, lv_color_t);
  LvPPObjStyleProxySelect(BorderOpa, border_opa, lv_opa_t);
  LvPPObjStyleProxySelect(BorderWidth, border_width, lv_coord_t);
  LvPPObjStyleProxySelect(BorderSide, border_side, lv_border_side_t);
  LvPPObjStyleProxySelect(BorderPost, border_post, bool);

  LvPPObjStyleProxySelect(TextColor, text_color, lv_color_t);
  LvPPObjStyleProxySelect(TextColorFiltered, text_color_filtered, lv_color_t);
  LvPPObjStyleProxySelect(TextOpa, text_opa, lv_opa_t);
  LvPPObjStyleProxySelect(TextFont, text_font, const lv_font_t*);
  LvPPObjStyleProxySelect(TextDecor, text_decor, lv_text_decor_t);
  LvPPObjStyleProxySelect(TextAlign, text_align, lv_text_align_t);

  LvPPObjStyleProxySelect(ImgOpa, img_opa, lv_opa_t);
  LvPPObjStyleProxySelect(ImgRecolor, img_recolor, lv_color_t);
  LvPPObjStyleProxySelect(ImgRecolorFiltered, img_recolor_filtered, lv_color_t);
  LvPPObjStyleProxySelect(ImgRecolorOpa, img_recolor_opa, lv_opa_t);

  LvPPObjStyleProxySelect(OutlineWidth, outline_width, lv_coord_t);
  LvPPObjStyleProxySelect(OutlineColor, outline_color, lv_color_t);
  LvPPObjStyleProxySelect(OutlineColorFiltered, outline_color_filtered, lv_color_t);
  LvPPObjStyleProxySelect(OutlineOpa, outline_opa, lv_opa_t);
  LvPPObjStyleProxySelect(OutlinePad, outline_pad, lv_coord_t);
};
