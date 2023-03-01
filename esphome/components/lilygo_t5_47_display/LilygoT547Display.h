#pragma once
#include "esphome/core/component.h"

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "esphome/core/hal.h"
#include "esphome/core/version.h"

#ifndef EPD_DRIVER
#define EPD_DRIVER
#include "epd_driver.h"
#include "epd_highlevel.h"
#endif

namespace esphome {
namespace lilygo_t5_47_display {

// LilyGo-EPD47
class LilygoT547Display : public PollingComponent, public display::DisplayBuffer {
 public:
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2022, 6, 0)
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
#endif

  void set_clear_screen(bool val) { this->clear_screen_ = val; }
  void set_landscape(bool val) { this->landscape_ = val; }
  void set_power_off_delay_enabled(bool val) { this->power_off_delay_enabled_ = val; }
  void set_temperature(uint8_t val) { this->temperature_ = val; }
  void set_full_update_every(uint8_t val) { this->full_update_every_ = val; }

  int get_width_internal() override { return 960; }
  int get_height_internal() override { return 540; }

  void setup() override;
  void update() override;

  void set_all_white() { epd_hl_set_all_white(&hl); }
  void poweron() { epd_poweron(); }
  void poweroff() { epd_poweroff(); }
  void on_shutdown() override;

 protected:
  void HOT draw_absolute_pixel_internal(int x, int y, Color color) override;

  void clear_();
  void flush_screen_changes_();

  bool init_clear_executed_ = false;
  EpdiyHighlevelState hl;
  uint8_t *fb;
  enum EpdDrawError err;

  bool clear_screen_;
  bool landscape_;
  bool power_off_delay_enabled_;
  // ambient temperature around device
  uint8_t temperature_;
  uint8_t full_update_every_;
  uint32_t partial_updates_{0};
};

}  // namespace lilygo_t5_47_display
}  // namespace esphome
