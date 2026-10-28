// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <Arduino.h>
#include <DigiKeyboard.h>

// The original firmware used plain INPUT and treated a HIGH reading as
// pressed, so the existing pedal is wired active-high (button to +5 V, with a
// pull-down). A fresh build should use active-low: button to GND and the
// internal pull-up. Set this to true for that wiring.
constexpr bool BUTTON_ACTIVE_LOW = false;

// Ignore contact bounce for this long after the last edge.
constexpr uint16_t DEBOUNCE_MS = 25;

struct KeyBinding {
  uint8_t pin;
  uint16_t key;
  uint16_t modifier;
};

// Button 0 -> Space
// Button 1 -> X
// Button 2 -> Left Ctrl
// Button 5 -> Left Shift
//
// Change the key and modifier values here. Use the KEY_* and MOD_* constants
// from DigiKeyboard.h. A standalone modifier uses key 0. For a shifted
// character, use the base KEY_* constant and put MOD_SHIFT_LEFT in the
// modifier field.
constexpr KeyBinding KEY_BINDINGS[] = {
    {0, KEY_SPACE, 0},
    {1, KEY_X, 0},
    {2, 0, MOD_CONTROL_LEFT},
    {5, 0, MOD_SHIFT_LEFT},
};

constexpr uint8_t BUTTON_COUNT =
    static_cast<uint8_t>(sizeof(KEY_BINDINGS) / sizeof(KEY_BINDINGS[0]));
