#include <avrm.h>
#include <avrm/shift.h>

#define ROWS 3

int main(void) {
    // Setup the shift register array.
    ShiftLatchConfig config = { 11, 12, 8 };
    shift_latch_init(config);

    // `io` contains the two dimensions of an individual LED layer.
    // The format is as follows:
    // [ anode pin (MUST only be one bit set at a time),
    //   cathode config (ANY arrangement of pins may be used) ]
    // As dictated by hardware, these config registers are active LOW,
    // meaning a value of 0x00 in both the anode and cathode config
    // would turn the entire matrix on at once (don't do this).
    byte io[2];

    for (;;) {
        // Each LED one at a time.
        for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < 8; j++) {
            io[0] = ~(0x01<<i);
            io[1] = ~(0x01<<j);
            shift_latch(config, io, 2);
            delay_ms(50);
        }
        // Each row at a time.
        for (int i = 0; i < ROWS; i++) {
            io[0] = ~(0x01<<i);
            // NOTE: This is safe to completly enable because each cathod is
            // attached to a pin able to sink it's ~20mA.
            io[1] = 0x00;
            shift_latch(config, io, 2);
            delay_ms(400);
        }
        // Fast scanning.
        for (int n = 0; n < 3000; n++)
        for (int i = 0; i < ROWS; i++) {
            io[0] = ~(0x01<<i);
            io[1] = 0x42;
            shift_latch(config, io, 2);
        }
    }

    return 0;
}

