# Exercise 1 — GPIO interrupt

Firmware for the NUCLEO-L053R8 (`ststm32` / CMSIS). The active code configures the user button on **PC13** as a falling-edge EXTI interrupt and toggles the on-board **PA5 LED** in `EXTI4_15_IRQHandler`. UART is initialized at 115200 baud, but the UART demonstrations in the main loop are commented out.

## Run

```sh
pio run
pio run --target upload
```

Press the Nucleo user button to toggle the LED. See [`src/main.c`](src/main.c) for the GPIO and interrupt setup. `README-template.md` contains the original project template instructions.
