# Project 3 — Aufgabe 2 RC signal variant

This NUCLEO-L053R8 branch captures an analog signal on **PC5 / ADC1 channel 15** and plots it on an **ILI9341** display. In this variant, the **TIM2** sampling interrupt also uses ADC thresholds to drive **PC4/PC8** for RC signal control. Unlike the [`proj_3`](https://github.com/renzhonglu11/PMI/tree/proj_3) branch, `initialize_project()` does not start TIM6 for a separate timer-generated signal.

The ADC data goes into a **240-sample** circular buffer. Capture is centered on a falling threshold crossing, with **120 samples before** and **120 after** the trigger. **TIM21** debounces the **PB1/PB2** zoom buttons. The main loop draws the graph and metrics when `graph_ready` is set, then restarts sampling. See [`src/timer/timer.c`](src/timer/timer.c) for the RC/trigger logic and [`src/main.c`](src/main.c) for the display loop.

## Build and upload

```sh
pio run
pio run --target upload
```

The PlatformIO environment is `nucleo_l053r8` (CMSIS). Connect the analog circuit, buttons and display according to the source before running on hardware.
