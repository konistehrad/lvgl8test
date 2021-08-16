#pragma once

#include <lvgl.h>

#define LvPPStyleProxySimple(lvname, lvproptype) \
  void lvname(lvproptype val) { lv_style_set_##lvname(&m_Style, val); }

#define LvPPStyleProxy(lvcamelname, lvunderscorename, lvproptype) \
  void lvcamelname(lvproptype val) { lv_style_set_##lvunderscorename(&m_Style, val); }

class LvStyle {
protected:
  lv_style_t m_Style;
public:
  LvStyle() { lv_style_init(&m_Style); }
  operator lv_style_t*() { return &m_Style; }

  LvPPStyleProxySimple(width, lv_coord_t);
  LvPPStyleProxy(minWidth, min_width, lv_coord_t);
  LvPPStyleProxy(maxWidth, max_width, lv_coord_t);

  LvPPStyleProxySimple(height, lv_coord_t);
  LvPPStyleProxy(minHeight, min_height, lv_coord_t);
  LvPPStyleProxy(maxHeight, max_height, lv_coord_t);

  LvPPStyleProxySimple(x, lv_coord_t);
  LvPPStyleProxySimple(y, lv_coord_t);
  LvPPStyleProxySimple(align, lv_align_t);

  LvPPStyleProxy(transformWidth, transform_width, lv_coord_t);
  LvPPStyleProxy(transformHeight, transform_height, lv_coord_t);

  LvPPStyleProxy(translateX, translate_x, lv_coord_t);
  LvPPStyleProxy(translateY, translate_y, lv_coord_t);

  LvPPStyleProxy(transformZoom, transform_zoom, lv_coord_t);
  LvPPStyleProxy(transformAngle, transform_angle, lv_coord_t);

  LvPPStyleProxy(padTop, pad_top, lv_coord_t);
  LvPPStyleProxy(padBottom, pad_bottom, lv_coord_t);
  LvPPStyleProxy(padLeft, pad_left, lv_coord_t);
  LvPPStyleProxy(padRight, pad_right, lv_coord_t);
  LvPPStyleProxy(padRow, pad_row, lv_coord_t);
  LvPPStyleProxy(padColumn, pad_column, lv_coord_t);

  LvPPStyleProxySimple(radius, lv_coord_t);
  LvPPStyleProxy(clipCorner, clip_corner, bool);

  LvPPStyleProxySimple(opa, lv_opa_t);
  LvPPStyleProxy(colorFilterDsc, color_filter_dsc, lv_color_filter_dsc_t*);
  LvPPStyleProxy(colorFilterOpa, color_filter_opa, lv_opa_t);

  LvPPStyleProxy(animTime, anim_time, uint32_t);
  LvPPStyleProxy(animSpeed, anim_speed, uint32_t);
  LvPPStyleProxySimple(transition, lv_style_transition_dsc_t*);

  LvPPStyleProxy(blendMode, blend_mode, lv_blend_mode_t);
  LvPPStyleProxySimple(layout, uint16_t);
  LvPPStyleProxy(baseDir, base_dir, lv_base_dir_t);

  LvPPStyleProxy(bgColor, bg_color, lv_color_t);
  LvPPStyleProxy(bgColorFiltered, bg_color_filtered, lv_color_t);
  LvPPStyleProxy(bgOpa, bg_opa, lv_opa_t);
  LvPPStyleProxy(bgGradColor, bg_grad_color, lv_color_t);
  LvPPStyleProxy(bgGradColorFiltered, bg_grad_color_filtered, lv_color_t);
  LvPPStyleProxy(bgGradDir, bg_grad_dir, lv_grad_dir_t);
  LvPPStyleProxy(bgImgSrc, bg_img_src, const void*);
  LvPPStyleProxy(bgImgOpa, bg_img_opa, lv_opa_t);
  LvPPStyleProxy(bgImgRecolor, bg_img_recolor, lv_color_t);
  LvPPStyleProxy(bgImgRecolorFiltered, bg_img_recolor_filtered, lv_color_t);
  LvPPStyleProxy(bgImgRecolorOpa, bg_img_recolor_opa, lv_opa_t);
  LvPPStyleProxy(bgImgTiled, bg_img_tiled, bool);

  LvPPStyleProxy(textColor, text_color, lv_color_t);
  LvPPStyleProxy(textColorFiltered, text_color_filtered, lv_color_t);
  LvPPStyleProxy(textOpa, text_opa, lv_opa_t);
  LvPPStyleProxy(textFont, text_font, const lv_font_t*);
  LvPPStyleProxy(textDecor, text_decor, lv_text_decor_t);
  LvPPStyleProxy(textAlign, text_align, lv_text_align_t);

  LvPPStyleProxy(imgOpa, img_opa, lv_opa_t);
  LvPPStyleProxy(imgRecolor, img_recolor, lv_color_t);
  LvPPStyleProxy(imgRecolorFiltered, img_recolor_filtered, lv_color_t);
  LvPPStyleProxy(imgRecolorOpa, img_recolor_opa, lv_opa_t);

  bool empty() { return lv_style_is_empty(&m_Style); }
};
