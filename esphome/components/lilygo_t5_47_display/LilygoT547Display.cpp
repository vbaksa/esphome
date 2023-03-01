#include "LilygoT547Display.h"
#include "esphome/core/log.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

namespace esphome {
namespace lilygo_t5_47_display {

static const char *const TAG = "lilygo_t5_47_display";

void LilygoT547Display::setup() {
  epd_init(EPD_OPTIONS_DEFAULT);
  this->hl = epd_hl_init(WAVEFORM);
  if (landscape_) {
    EpdRotation orientation = EPD_ROT_LANDSCAPE;
    epd_set_rotation(orientation);
  } else {
    EpdRotation orientation = EPD_ROT_PORTRAIT;
    epd_set_rotation(orientation);
  }
  this->fb = epd_hl_get_framebuffer(&this->hl);
  this->partial_updates_ = this->full_update_every_;
}

void LilygoT547Display::update() {
  epd_poweron();
  if (this->init_clear_executed_ == false && this->clear_screen_ == true) {
    this->clear_();
    this->init_clear_executed_ = true;
  } else {
    if (this->partial_updates_ == 0 && this->full_update_every_ != 0) {
      this->clear_();
    } else {
      this->partial_updates_--;
    }
  }
  // Clear Framebuffer
  epd_hl_set_all_white(&this->hl);

  this->do_update_();
  this->flush_screen_changes_();
  if (this->power_off_delay_enabled_ == true) {
    delay(700);
  }
  epd_poweroff();
}

void LilygoT547Display::clear_() {
  this->partial_updates_ = this->full_update_every_;
  epd_fullclear(&this->hl, this->temperature_);
}

void LilygoT547Display::flush_screen_changes_() { this->err = epd_hl_update_screen(&this->hl, MODE_GC16, this->temperature_); }

void LilygoT547Display::on_shutdown() {
  ESP_LOGI(TAG, "Shutting down Lilygo T5-4.7 screen");
  epd_poweroff();
  epd_deinit();
}

void HOT LilygoT547Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (color.red == 255 && color.green == 255 && color.blue == 255) {
    epd_draw_pixel(x, y, 0, this->fb);
  } else {
    int col = (0.2126 * color.red) + (0.7152 * color.green) + (0.0722 * color.blue);
    int cl = 255 - col;
    epd_draw_pixel(x, y, cl, this->fb);
  }
}

}  // namespace lilygo_t5_47_display
}  // namespace esphome
