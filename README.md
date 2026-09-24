# PMI — Nucleo-L053R8 exercises and projects

This repository contains separate PlatformIO/CMSIS implementations for the PMI exercises and three embedded projects. **Choose a branch before building:** each branch has its own `src/main.c` and hardware setup. The `main` branch is an index and retains an older exercise snapshot; it is not the combined application.

## Branches

| Branch | Contents |
| --- | --- |
| [`exe_1`](https://github.com/renzhonglu11/PMI/tree/exe_1) | PC13 button interrupt toggles the PA5 LED. |
| [`exe_2`](https://github.com/renzhonglu11/PMI/tree/exe_2) | Array maximum and integer-to-string exercises with UART output. |
| [`exe_3`](https://github.com/renzhonglu11/PMI/tree/exe_3) | TIM2 PWM duty-cycle control through serial commands `1` and `0`. |
| [`proj_1`](https://github.com/renzhonglu11/PMI/tree/proj_1) | Traffic-light controller with MCP23017, LCD and UART receive timeout indication. |
| [`proj_1_uart_master`](https://github.com/renzhonglu11/PMI/tree/proj_1_uart_master) | Companion UART sender for Project 1. |
| [`proj_2`](https://github.com/renzhonglu11/PMI/tree/proj_2) | ADXL345 accelerometer, QMC5883L magnetometer and DS18B20 temperature sensor. |
| [`proj_3`](https://github.com/renzhonglu11/PMI/tree/proj_3) | Timer-generated signal and ADC oscilloscope with LCD graph and zoom buttons. |
| [`proj_3_aufgabe2`](https://github.com/renzhonglu11/PMI/tree/proj_3_aufgabe2) | Project 3 Aufgabe 2 variant, using ADC readings for RC signal control. |

## Build and upload

Install [PlatformIO](https://platformio.org/) and connect a NUCLEO-L053R8. From the repository root:

```sh
git switch proj_2              # replace with the branch you want
pio run
pio run --target upload
pio device monitor             # when the branch uses serial output
```

Every branch defines `env:nucleo_l053r8` with the `ststm32` platform and CMSIS framework in `platformio.ini`. The serial monitor is configured for 115200 baud. Project branches require their corresponding external sensors, display or wiring; see their README files.

## Demonstration videos

- [Project 1](Videos/video1.mov)
- [Project 2](Videos/video2.mov)
- [Project 3](Videos/video3.mov)

The original starter instructions are kept in [README-template.md](README-template.md).
