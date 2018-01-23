# LED Cube

[![Build Status](https://travis-ci.org/nixpulvis/led-cube.svg?branch=master)](https://travis-ci.org/nixpulvis/led-cube)

A handmade, delightful 8x8x8 red LED cube.
## Usage

First install [avrm](https://github.com/nixpulvis/avrm), then from this
repository run the following:

```sh
# The `env AVRDUDE_BAUD=XXX` may not be required for Arduino Pro Minis.
sudo env AVRDUDE_BAUD=115200 make main.flash
```
