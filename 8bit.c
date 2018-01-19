#include <avr/io.h>
#include <avrm.h>

void shift(uint8_t n);
void latch_shift(uint8_t n);

int main(void) {
    DDRB = DDRB | 0b00011001;

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (uint8_t i = 0; i < 256; i++) {
            latch_shift(i);
            delay_ms(100);
        }
    }

    return 0;
}

void shift(uint8_t n) {
    for (int i = 0; i < 8; i++) {
        // Write the current bit to the SER.
        if (n & 0b10000000) {
            PORTB |= _BV(3);
        } else {
            PORTB &= ~_BV(3);
        }

        // Toggle the SRCLK.
        PORTB |= _BV(4);
        PORTB &= ~_BV(4);

        // Advance the value to shift out.
        n <<= 1;
    }
}

void latch_shift(uint8_t n) {
    // Begin the shift by latching RCLK (logic LOW).
    PORTB &= ~_BV(0);
    // Shift out the value of `i`.
    shift(n);
    // End the shift be releasing RCLK (logic HIGH).
    PORTB |= _BV(0);
}

