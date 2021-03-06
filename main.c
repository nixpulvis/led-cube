#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrm.h>
#include <avrm/shift.h>

#define PROGRAM 2

// #define SCAN_DELAY 1000 / 8
#define UPDATE_RATE 60000

// `display` contains an 8x8 array of booleans for the state of the
// corrisponding LED. The origin is the bottom left.
static bool display[8][8];

void update();
void scan(ShiftLatchConfig, bool[8][8]);
byte to_byte(bool[8]);
void from_byte(bool[8], byte b);

ISR(TIMER1_OVF_vect) {
    // Update the display buffer.
    update();
    // Reset the timer.
    TCNT1 = UPDATE_RATE;
}

int main(void) {
    // Setup the ISR timer.
    TCNT1  = UPDATE_RATE;
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10) | (1<<CS12);
    TIMSK1 = (1 << TOIE1);
    sei();

    // Setup the inital display state.
#if PROGRAM == 0
    display[0][0] = TRUE;
#elif PROGRAM == 1
    for (int i = 0; i < 8; i++)
        from_byte(display[i], 0xFF);
#endif
    update();

    // Setup the shift register array.
    ShiftLatchConfig config = { 5, 6, 7 };
    shift_latch_init(config);

    for (;;) {
        scan(config, display);
    }

    return 0;
}

void update() {
#if PROGRAM == 0
    byte new[8][8];
    for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
        new[i][j] = display[i][j - 1];
    memcpy(display, new, sizeof(display));

    if (display[2][1]) {
        display[0][0] = TRUE;
    }
#elif PROGRAM == 1
    // noop.
#elif PROGRAM == 2
    bool new[8][8];
    for (int i = 0; i < 8; i++)
        from_byte(new[i], rand());
    memcpy(display, new, sizeof(display));
#endif
}

void scan(ShiftLatchConfig config, bool display[8][8]) {
    // `io` contains the two dimensions of an individual LED layer.
    // The format is as follows:
    // [ anode pin (MUST only be one bit set at a time),
    //   cathode config (ANY arrangement of pins may be used) ]
    // As dictated by hardware, these config registers are active LOW,
    // meaning a value of 0x00 in both the anode and cathode config
    // would turn the entire matrix on at once (don't do this).
    byte io[2];

    for (int i = 0; i < 8; i++) {
        // The anode byte.
        io[0] = ~(0x01<<i);

        // The cathode byte.
        io[1] = ~(to_byte(display[i]));

        // Shift this row of the scanned display.
        shift_latch(config, io, 2);
#ifdef SCAN_DELAY
        delay_ms(SCAN_DELAY);
#endif
    }
}

byte to_byte(bool b[8]) {
    unsigned char c = 0;
    for (int i=0; i < 8; ++i)
        if (b[i])
            c |= 1 << i;
    return c;
}

void from_byte(bool arr[8], byte b) {
    for (int i=0; i < 8; ++i)
        arr[i] = (b & (1<<i)) != 0;
}
