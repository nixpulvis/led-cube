#include <avr/io.h>
#include <avrm.h>
#include <avrm/port.h>

void shift(int data_pin, int clock_pin, uint8_t n);
void latch_shift(int latch_pin, int data_pin, int clock_pin, uint8_t n);

int main(void) {
    DDRB = DDRB | 0b00011001;

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (uint8_t i = 0; i < 256; i++) {
            latch_shift(8, 11, 12, i);
            delay_ms(100);
        }
    }

    return 0;
}

void shift(int data_pin, int clock_pin, uint8_t n) {
    for (int i = 0; i < 8; i++) {
        // Write the current bit to the SER.
        if (n & 0b10000000) {
            pin_out(data_pin, TRUE);
        } else {
            pin_out(data_pin, FALSE);
        }

        // Toggle the SRCLK.
        pin_out(clock_pin, TRUE);
        pin_out(clock_pin, FALSE);

        // Advance the value to shift out.
        n <<= 1;
    }
}

void latch_shift(int latch_pin, int data_pin, int clock_pin, uint8_t n) {
    // Begin the shift by latching RCLK (logic LOW).
    pin_out(latch_pin, FALSE);
    // Shift out the value of `i`.
    shift(data_pin, clock_pin, n);
    // End the shift be releasing RCLK (logic HIGH).
    pin_out(latch_pin, TRUE);
}

