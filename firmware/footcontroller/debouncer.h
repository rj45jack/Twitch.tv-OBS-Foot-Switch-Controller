// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <stdint.h>

// Debounces one button and reports a single event on the press edge.
//
// The class has no Arduino dependency, so the timing behaviour is covered by
// host-side tests. Feed it a raw "pressed" boolean and a millisecond
// timestamp; it returns true once when the input has been stable long enough.
class Debouncer {
 public:
  explicit Debouncer(uint16_t debounceMs = 0, bool initial = false)
      : debounceMs_(debounceMs),
        stable_(initial),
        lastRaw_(initial),
        lastChangeMs_(0) {}

  bool update(bool raw, uint32_t now) {
    if (raw != lastRaw_) {
      lastRaw_ = raw;
      lastChangeMs_ = now;
    }

    if (static_cast<uint32_t>(now - lastChangeMs_) >= debounceMs_ &&
        raw != stable_) {
      stable_ = raw;
      return stable_;
    }

    return false;
  }

 private:
  uint16_t debounceMs_;
  bool stable_;
  bool lastRaw_;
  uint32_t lastChangeMs_;
};
