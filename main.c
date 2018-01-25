#include <limits.h>
#include <avrm.h>
#include <avrm/shift.h>

#define ROWS 8

int main(void) {
    // Setup the shift register array.
    ShiftLatchConfig config = { 5, 6, 7 };
    shift_latch_init(config);

    // `io` contains the two dimensions of an individual LED layer.
    // The format is as follows:
    // [ anode pin (MUST only be one bit set at a time),
    //   cathode config (ANY arrangement of pins may be used) ]
    // As dictated by hardware, these config registers are active LOW,
    // meaning a value of 0x00 in both the anode and cathode config
    // would turn the entire matrix on at once (don't do this).
    byte io[2];

    // `count` a number we're baseing the display on.
    byte count = 0;

    // An incremental number to base count on.
    int n = 0;

    for (;;) {
        if (n % 50 == 0)
            count++;

        for (int i = 0; i < ROWS; i++) {
            io[0] = ~(0x01<<i);
            if (i % 2 == 0)
                io[1] = count;
            else
                io[1] = 255 - count;
            shift_latch(config, io, 2);
        }

        n++;
    }

    return 0;
}

