# Self-Calibrating STM32 Clock

*Note - based off an STM32G031K8 STM32CubeMX project made for a `makefile` building process. An external repository is also required, which can be found at `yani-again/simple_hal`.*

## Motivation

During the time I worked with a GNSS module for my university's rocketry team, I found out that GPS/GNSS receivers receive the current UTC time from satellites. That made me think about how reliable modern phones are with their time calibration - across all Android and iOS phones I've seen, they all seem to be synced to the exact same time down to the second.

However, for old-fashioned people who prefer physical clocks *(like myself)*, firing up an entire GNSS module just to get the time is wasteful for the battery and - quite frankly - it's over-engineering.

This led me to considering possibilities for a self-calibrating physical clock, one that wouldn't require manual configuring every time the battery dies *(childhood memories from my grandma's house...)*

*And that's how the idea for this project was born!*

The core idea is based off [**the MSF signal**](https://en.wikipedia.org/wiki/Time_from_NPL_(MSF)), which is a radio signal broadcasted by Babcock International. The signal contains the current national time and reaches anywhere in the country, so obviously this clock idea only works in the UK.

## Pin Layout

- **GPIO port B:** used for the LED screens and their relevant functions
- **GPIO port F:** used for the longwave radio module

## Notable Components

- [Nucleo-STM32G031K8](https://www.digikey.co.uk/en/products/detail/stmicroelectronics/NUCLEO-G031K8/10321671) *(1x)*
- [60KHz radio transceiver](https://www.xtals.co.uk/product-page/wwvb-60khz-single-frequency-modules-radio-time-signal-receiver) *(1x)*
- [SN74HC595N shift registers](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC595N/277246) *(4x)*
- [10-pin, 7-segment displays](https://www.amazon.co.uk/sourcing-map-Segment-Display-Electronic/dp/B0GYFY3G42?th=1) *(4x)*
