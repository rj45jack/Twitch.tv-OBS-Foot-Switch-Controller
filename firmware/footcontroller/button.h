// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <Arduino.h>

#include "debouncer.h"

// One physical button. begin() configures the pin and pressed() returns true
// once per debounced press, never on release and never repeatedly while held.
class Button {
 public:
  Button() = default;

  void begin(uint8_t pin, bool activeLow, uint16_t debounceMs) {
    pin_ = pin;
    activeLow_ = activeLow;
    debouncer_ = Debouncer(debounceMs);
    pinMode(pin_, activeLow_ ? INPUT_PULLUP : INPUT);
  }

  bool pressed(uint32_t now) {
    const bool raw = digitalRead(pin_) == (activeLow_ ? LOW : HIGH);
    return debouncer_.update(raw, now);
  }

 private:
  uint8_t pin_ = 0;
  bool activeLow_ = false;
  Debouncer debouncer_;
};
