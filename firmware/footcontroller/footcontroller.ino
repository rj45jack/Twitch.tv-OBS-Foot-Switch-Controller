// SPDX-License-Identifier: Apache-2.0
//
// Twitch.tv / OBS foot switch controller.
//
// Original sketch: rj45jack, 2017.
// Modern rewrite: 2026.
//
// Target: DigiSpark (ATtiny85 at 16.5 MHz) with four momentary buttons.
// The Digistump core's DigiKeyboard library turns the board into a standard
// USB keyboard, so it works with OBS Studio, Twitch, or any other application
// that accepts hotkeys.

#include <Arduino.h>
#include <DigiKeyboard.h>

#include "button.h"
#include "config.h"

Button buttons[BUTTON_COUNT];

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    buttons[i].begin(KEY_BINDINGS[i].pin, BUTTON_ACTIVE_LOW, DEBOUNCE_MS);
  }
}

void loop() {
  // V-USB needs to be polled continuously, even while the pedal is idle.
  DigiKeyboard.update();

  const uint32_t now = millis();
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    if (buttons[i].pressed(now)) {
      // sendKeyStroke sends a complete press/release pair and waits for the
      // USB interrupt to be ready between them, so the host cannot miss the
      // press. Casting drops the layout bits from the KEY_* macros; the low
      // byte is the HID usage code.
      DigiKeyboard.sendKeyStroke(static_cast<uint8_t>(KEY_BINDINGS[i].key),
                                 static_cast<uint8_t>(KEY_BINDINGS[i].modifier));
    }
  }
}
