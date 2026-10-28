// SPDX-License-Identifier: Apache-2.0
#include <unity.h>

#include "debouncer.h"

void test_bounce_settles_to_press(void) {
  Debouncer debouncer(25);
  TEST_ASSERT_FALSE(debouncer.update(false, 0));
  TEST_ASSERT_FALSE(debouncer.update(true, 10));
  TEST_ASSERT_FALSE(debouncer.update(false, 15));
  TEST_ASSERT_FALSE(debouncer.update(true, 20));
  TEST_ASSERT_FALSE(debouncer.update(true, 44));
  TEST_ASSERT_TRUE(debouncer.update(true, 45));
}

void test_held_button_fires_once(void) {
  Debouncer debouncer(20);
  TEST_ASSERT_FALSE(debouncer.update(true, 0));
  TEST_ASSERT_TRUE(debouncer.update(true, 20));
  TEST_ASSERT_FALSE(debouncer.update(true, 100));
  TEST_ASSERT_FALSE(debouncer.update(true, 1000));
}

void test_release_arms_the_next_press(void) {
  Debouncer debouncer(20);
  TEST_ASSERT_FALSE(debouncer.update(true, 0));
  TEST_ASSERT_TRUE(debouncer.update(true, 20));

  TEST_ASSERT_FALSE(debouncer.update(false, 30));
  TEST_ASSERT_FALSE(debouncer.update(false, 50));

  TEST_ASSERT_FALSE(debouncer.update(true, 60));
  TEST_ASSERT_TRUE(debouncer.update(true, 80));
}

void test_bounce_never_triggers_early(void) {
  Debouncer debouncer(30);
  for (uint32_t t = 0; t <= 90; t += 5) {
    const bool raw = ((t / 5) % 2) == 0;
    TEST_ASSERT_FALSE(debouncer.update(raw, t));
  }

  TEST_ASSERT_FALSE(debouncer.update(true, 119));
  TEST_ASSERT_TRUE(debouncer.update(true, 120));
}

void test_zero_debounce_passes_through(void) {
  Debouncer debouncer(0);
  TEST_ASSERT_TRUE(debouncer.update(true, 0));
  TEST_ASSERT_FALSE(debouncer.update(true, 10));
  TEST_ASSERT_FALSE(debouncer.update(false, 20));
  TEST_ASSERT_TRUE(debouncer.update(true, 30));
}

void test_millis_wraparound(void) {
  Debouncer debouncer(20);
  TEST_ASSERT_FALSE(debouncer.update(true, 0xFFFFFFF0u));
  TEST_ASSERT_TRUE(debouncer.update(true, 0x00000004u));
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_bounce_settles_to_press);
  RUN_TEST(test_held_button_fires_once);
  RUN_TEST(test_release_arms_the_next_press);
  RUN_TEST(test_bounce_never_triggers_early);
  RUN_TEST(test_zero_debounce_passes_through);
  RUN_TEST(test_millis_wraparound);
  return UNITY_END();
}
