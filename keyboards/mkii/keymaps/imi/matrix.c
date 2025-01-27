#include "matrix.h"
#include "quantum.h"

#define MATRIX_ROWS 4
#define MATRIX_COLS 8

// Define your multiplexer pins
uint8_t mux_pins[] = {D0, D1, D2}; // S0, S1, S2 pins

void set_mux_channel(int channel) {
    digitalWrite(mux_pins[0], (channel & 1) ? HIGH : LOW);
    digitalWrite(mux_pins[1], ((channel >> 1) & 1) ? HIGH : LOW);
    digitalWrite(mux_pins[2], ((channel >> 2) & 1) ? HIGH : LOW);
}

void matrix_init_custom(void) {
    // initialize multiplexer pins
    for (int i = 0; i < sizeof(mux_pins); i++) {
        setPinOutput(mux_pins[i]);
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_changed = false;
    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int channel = 0; channel < 8; channel++) {
            set_mux_channel(channel);
            int pin_state = readPin(row_pins[row]);
            current_matrix[row] |= (pin_state ? (1 << channel) : 0);
            matrix_changed |= (pin_state != matrix_get_row(row) & (1 << channel));
        }
    }
    return matrix_changed;
}

matrix_row_t matrix_get_row(uint8_t row) {
    return matrix_debouncing[row];
}

void matrix_power_down(void) {
    // Custom power down routine
}