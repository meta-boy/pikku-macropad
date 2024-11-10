// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum keycodes {
  KC_CYCLE_LAYERS = QK_USER,
};

// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   2

// Add the behaviour of this new keycode
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_CYCLE_LAYERS:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) { 
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return false;
      }

      uint8_t next_layer = current_layer + 1;
      if (next_layer > LAYER_CYCLE_END) {
          next_layer = LAYER_CYCLE_START;
      }
      layer_move(next_layer);
      return false;

    // Process other keycodes normally
    default:
      return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ A   │ B   │Cycle│
     * ├───┼───┼───┤
     * │ C   │ D   │ E   │
     * └───┴───┴───┘
     */
    [0] = LAYOUT_ortho_2x3(
        KC_A,    KC_B,    KC_CYCLE_LAYERS,
        KC_C,    KC_D,    KC_E
    )
};



#ifdef OLED_ENABLE

static void render_layer_state(void) {
    oled_write_P(PSTR("\n\nLayer: "), false);
    
    // Convert the layer number to a string and display it
    char layer_str[2] = {0};
    uint8_t current_layer = get_highest_layer(layer_state);
    layer_str[0] = current_layer + '0';  // Convert number to ASCII
    oled_write(layer_str, false);
}

bool oled_task_user(void) {
    render_layer_state();
    return false;
}
#endif