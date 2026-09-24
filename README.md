# Exercise 2 — arrays, pointers and UART output

Firmware for the NUCLEO-L053R8 (`ststm32` / CMSIS). `src/main.c` adds functions to find the maximum of a signed 16-bit array, return a pointer to that element, print an array, and convert an integer to text.

The active loop finds the maximum of `{0, 123, 42, -399}`, changes that element to `1337`, and prints the converted value over the Nucleo USB serial bridge every second. The **PC13** button interrupt from Exercise 1 still toggles the **PA5** LED. Other UART and array examples in the source are commented out.

## Run

```sh
pio run
pio run --target upload
pio device monitor
```

The serial monitor uses **115200 baud**. The expected repeated output is `1337`. See [`src/main.c`](src/main.c) for the exercise functions.
