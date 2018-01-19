#include <avr/io.h>
#include <avrm.h>
#include <avrm/pin.h>
#include <avrm/shift.h>

int main(void) {
    // Set the control pins as outputs.
    pin_mode(8, OUTPUT);
    pin_mode(11, OUTPUT);
    pin_mode(12, OUTPUT);

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (uint8_t i = 0; i < 256; i++) {
            shift_latch(11, 12, 8, i);
            delay_ms(100);
        }
    }

    return 0;
}

