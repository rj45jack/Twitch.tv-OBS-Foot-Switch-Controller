# Twitch.tv Foot Switch Controller

A four-button USB foot pedal built on a DigiSpark (ATtiny85). It enumerates as
a standard USB keyboard, so it works with OBS Studio, Twitch, or any other
application that accepts hotkeys. The original build was for a friend who
speed-runs Super Metroid.

Original demo video: <https://www.youtube.com/watch?v=Kfadq8JCBqg>

[![firmware](https://github.com/rj45jack/Twitch.tv-OBS-Foot-Switch-Controller/actions/workflows/firmware.yml/badge.svg)](https://github.com/rj45jack/Twitch.tv-OBS-Foot-Switch-Controller/actions/workflows/firmware.yml)
[![license](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)

## What changed in this rewrite

- **DigiKeyboard instead of TrinketKeyboard.** The Digistump core already ships
  `DigiKeyboard`, which is built for the DigiSpark's 16.5 MHz clock. No
  external keyboard library is needed.
- **No dropped keypresses.** The original sent press and release back-to-back.
  `DigiKeyboard.sendKeyStroke()` waits for the USB interrupt to be ready
  between them, so the host sees the press before the release.
- **Debounced buttons.** 25 ms of debounce, and one key per physical press —
  holding a button no longer spams the host with repeats.
- **Ctrl and Shift fixed.** The original sent the modifier as both the
  modifier and the keycode (`KEYCODE_MOD_LEFT_CONTROL` +
  `KEYCODE_LEFT_CONTROL`), which is not a valid HID report. The rewrite sends
  the modifier alone.
- **Key map in `config.h`.** Change a key without touching the loop.
- **USB polled every iteration.** No blocking `delay()` in the main loop.
- **PlatformIO build, host-side debounce tests, and GitHub Actions CI.**

## Default key map

| Button | DigiSpark pin | Sends |
|---|---|---|
| 0 | P0 | Space |
| 1 | P1 | X |
| 2 | P2 | Left Ctrl |
| 5 | P5 | Left Shift |

The map is `KEY_BINDINGS` in `firmware/footcontroller/config.h`. For example,
Ctrl+S on button 2 is:

```cpp
{2, KEY_S, MOD_CONTROL_LEFT},
```

A standalone modifier uses key `0`. For a shifted character, use the base
`KEY_*` constant and put `MOD_SHIFT_LEFT` in the modifier field.

## Wiring

| Function | DigiSpark pin | ATtiny85 pin | Notes |
|---|---|---|---|
| Button 0 | P0 | PB0 | |
| Button 1 | P1 | PB1 | shares the onboard LED on a Model A |
| Button 2 | P2 | PB2 | |
| Button 5 | P5 | PB5 / RESET | see the warning below |
| USB D− | P3 | PB3 | do not use for buttons |
| USB D+ | P4 | PB4 | do not use for buttons |
| +5 V | 5V | VCC | |
| Ground | GND | GND | |

Full wiring notes, including the active-high and active-low options:
[`docs/wiring.md`](docs/wiring.md).

> **P5 is the RESET pin on a stock DigiSpark.** The fourth button only works as
> an input if the ATtiny85's `RSTDISBL` fuse has been programmed. With RESET
> enabled, active-high wiring reads as permanently pressed (the pin is held
> high internally) and active-low wiring resets the board; in neither case
> does button 4 work as a normal input. Some clones ship with the fuse already
> set, so check yours before assuming the fourth button works. Programming
> `RSTDISBL` is not reversible without a high-voltage programmer, so a
> DigiSpark Pro or a board with more I/O is the safer route if you need four
> buttons.
>
> **P1 drives the onboard LED** on a Model A. Button 1 shares that pin. With
> the original active-high wiring the LED acts as a weak pull-down and lights
> while the button is pressed. With active-low wiring the LED clamps the pin
> below the logic-high threshold, so P1 can read as permanently pressed; do
> not use active-low on P1 without removing the LED or its resistor.

### Button polarity

The original firmware used `pinMode(..., INPUT)` and treated `HIGH` as
pressed, so the existing pedal is wired active-high: each button connects the
pin to +5 V. P1 is pulled down by the onboard LED; P0 and P2 need an external
10 kΩ pull-down, and P5 needs one too after the reset fuse is disabled. That
is the default:

```cpp
constexpr bool BUTTON_ACTIVE_LOW = false;
```

Active-low is possible on P0, P2 and P5: wire each button to GND, set
`BUTTON_ACTIVE_LOW = true`, and the firmware enables the internal pull-up. Do
not use active-low on P1 without removing the onboard LED or its resistor:
the LED clamps the pin below the ATtiny85's logic-high threshold, so the input
can read as permanently pressed. Because the polarity setting is global, the
practical four-button build keeps the original active-high wiring and adds the
three pull-downs.

Debouncing is done in software; no capacitors are required.

## Build and flash

### PlatformIO

```bash
cd firmware
pio run                 # build
pio test -e native      # host-side debounce tests
pio run -t upload       # flash over the micronucleus bootloader
```

The upload uses the DigiSpark's micronucleus bootloader. `pio` will ask you to
plug the board in; the bootloader only waits a few seconds after power-up.

### Arduino IDE

1. Install the Digistump AVR board support (the DigiSpark board package).
2. Select **Digispark (Default - 16.5 MHz)**.
3. Open `firmware/footcontroller/footcontroller.ino` and upload.

The Digistump core includes `DigiKeyboard`, so there is no separate library to
install. The default keyboard layout is US English; the core's `keylayouts.h`
has other layouts if you need one. If the old Digistump package index no
longer loads, use PlatformIO; it fetches the same core automatically.

## Using it with OBS

OBS Studio treats the pedal as a normal keyboard:

1. **Settings → Hotkeys**.
2. Click the field for the scene or source you want, then press the foot
   button.
3. Close the settings window; OBS saves the binding.

The default keys are Space, X, Ctrl, and Shift. Change `KEY_BINDINGS` if those
conflict with something else.

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| Button 4 resets the board, or Shift fires continuously | P5 is RESET; `RSTDISBL` is not set. See above. |
| A button fires on its own | Input is floating; add the pull-down (active-high) or use the internal pull-up (active-low). |
| A button fires twice | Check `DEBOUNCE_MS`; 25 ms suits most tactile switches. |
| Keypresses are missed | The host may be slow to enumerate. Try another USB port and avoid unpowered hubs. |
| Device not detected | The DigiSpark is low-speed USB 1.1. Try a different cable or a direct port. |
| Wrong character on a non-US keyboard | The firmware uses the core's default US layout; change the layout in `keylayouts.h`. |

## Parts

- DigiSpark (generic ATtiny85 board)
- 4 momentary pushbuttons
- Wire, an enclosure, and something to stop it sliding across the floor
- A USB port or cable to power it

## Repository layout

```
firmware/
  platformio.ini
  footcontroller/
    footcontroller.ino
    config.h        pin map, key bindings, debounce time
    button.h        pin wrapper around the debouncer
    debouncer.h     host-testable debounce logic
  test/
    test_debouncer/ host tests for the debounce behaviour
docs/
  wiring.md
.github/workflows/firmware.yml
```

## Verification

- `pio test -e native` — 6/6 debounce tests pass, including contact bounce,
  held buttons, release/re-press, and `millis()` wraparound.
- `pio run -e digispark-tiny` — 2,978 bytes flash (49.5% of 6,012) and
  142 bytes RAM (27.7% of 512).

This is compile-tested and unit-tested, not hardware-tested. The original unit
is from 2017 and was not available to re-flash.

## License

Apache-2.0. See [`LICENSE`](LICENSE).
