



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

### Project 1
https://github.com/user-attachments/assets/0730b3e6-070b-4cec-83c8-760b549fa057
### Project 2
https://github.com/user-attachments/assets/dc5a2271-0f57-431f-8509-49320302d5f4
### Project 3
https://github.com/user-attachments/assets/0bcc3af0-fc46-461a-bc91-ed84b4d1b4d7


The original starter instructions are kept in [README-template.md](README-template.md).
