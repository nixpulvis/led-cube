#include <avr/io.h>
#include <avrm.h>
#include <avrm/shift.h>

int main(void) {
    // TODO: This needs to be updated to a function in avrm/pin.h.
    DDRB = DDRB | 25;

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (uint8_t i = 0; i < 256; i++) {
            shift_latch(11, 12, 8, i);
            delay_ms(100);
        }
    }

    return 0;
}

