#pragma once 

#include <lvgl.h>
#include "LvStyle.hpp"
#include "LvObj.hpp"

class SysBar: public LvObj {
private:
  LvObj m_Left;
  LvObj m_Center;
  LvObj m_Right;
  LvLabel m_TimeLabel;
  LvLabel m_WifiLabel;
  LvLabel m_BatteryLabel;
  LvStyle m_ContainerStyle;
public:
  SysBar(lv_coord_t barHeight = 30) : 
    LvObj(LvObj::layerTop(), &lv_obj_create),
    m_Left(m_RawObj),
    m_Center(m_RawObj),
    m_Right(m_RawObj),
    m_TimeLabel(m_Center),
    m_WifiLabel(m_Right),
    m_BatteryLabel(m_Right),
    m_ContainerStyle()
  { 
    removeStyleAll();
    styleBgColor(lv_color_black(), 0);
    styleBgOpa(LV_OPA_50, 0);
    styleTextColor(lv_color_white(), 0);
    width(LV_HOR_RES);
    height(barHeight);
    align(LV_ALIGN_TOP_MID);

    layout(LV_LAYOUT_FLEX);
    flexFlow(LV_FLEX_FLOW_ROW);

    m_ContainerStyle.padColumn(10);
    m_ContainerStyle.padLeft(10);
    m_ContainerStyle.padRight(10);
    m_ContainerStyle.height(barHeight);
    m_ContainerStyle.layout(LV_LAYOUT_FLEX);
    m_ContainerStyle.flexGrow(0);
    m_ContainerStyle.flexFlow(LV_FLEX_FLOW_ROW);
    m_ContainerStyle.flexCrossPlace(LV_FLEX_ALIGN_CENTER);
    m_ContainerStyle.flexMainPlace(LV_FLEX_ALIGN_START);
    m_ContainerStyle.flexTrackPlace(LV_FLEX_ALIGN_CENTER);

    m_Left.replaceStyle(m_ContainerStyle); 
    m_Center.replaceStyle(m_ContainerStyle);
    m_Right.replaceStyle(m_ContainerStyle);

    m_Right.flexAlign(LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    m_Center.flexGrow(1);
    m_Center.flexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    m_WifiLabel.text(LV_SYMBOL_WIFI);
    m_BatteryLabel.text(LV_SYMBOL_BATTERY_FULL);
    m_TimeLabel.text("13:42");
  }
};
