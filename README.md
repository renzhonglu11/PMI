# Exercise 3 — PWM controlled over UART

Firmware for the NUCLEO-L053R8 (`ststm32` / CMSIS). The active program configures **TIM2 channel 1** for PWM on **PA5** and waits for characters from the Nucleo USB serial bridge. Earlier interrupt and array examples remain in `src/main.c` but are not called by this loop.

## Run

```sh
pio run
pio run --target upload
pio device monitor
```

Use a **115200-baud** serial connection:

| Character | Action |
| --- | --- |
| `1` | Increase the TIM2 compare value by 1000, up to the configured limit. |
| `0` | Decrease the compare value by 1000, down to the configured limit. |

The program echoes each received character. PWM setup uses `TIM2->ARR = 10000` and starts with a compare value near 200; see [`src/main.c`](src/main.c) for the exact register writes and bounds.
