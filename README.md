# Project 3 — signal capture and LCD graph

This NUCLEO-L053R8 firmware samples an analog signal on **PC5 / ADC1 channel 15** and plots captured values on an **ILI9341** display. **TIM6** toggles the generated signal on PC4/PC8; **TIM2** drives ADC sampling; **TIM21** debounces the **PB1/PB2** zoom buttons. PB1 lowers and PB2 raises the zoom level within the range 1–5.

The capture logic in [`src/timer/timer.c`](src/timer/timer.c) stores **240** samples in a circular buffer, detects a falling crossing of the ADC threshold, and collects **120 pre-trigger** and **120 post-trigger** samples. Once a capture is ready, [`src/main.c`](src/main.c) draws the waveform and measured values, then resets capture. Graph and metric calculations live in `src/graph_utils/`.

## Build and upload

```sh
pio run
pio run --target upload
```

This branch is the timer-generated signal version. The [`proj_3_aufgabe2`](https://github.com/renzhonglu11/PMI/tree/proj_3_aufgabe2) branch instead adjusts the RC signal from ADC readings. The PlatformIO environment is `nucleo_l053r8` (CMSIS); connect the analog circuit, buttons and display according to the source before running on hardware.
