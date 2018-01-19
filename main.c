#include <avrm.h>
#include <avrm/shift.h>

int main(void) {
    // Setup the shift register array.
    ShiftLatchConfig config = { 11, 12, 8 };
    shift_latch_init(config);

    for (;;) {
        // Shift out the binary from 0 to 255 each 100ms.
        for (byte i = 0; i < 256; i++) {
            byte buff[2] = {
                i % 2 == 0 ? 1 : 2,
                i % 2 == 0 ? i : 255-i
            };

            // XXX: We need a way to delay the buff[1] loop, but not the
            // buff[0] loop.
            shift_latch(config, buff, 2);
            delay_ms(100);
        }
    }

    return 0;
}

