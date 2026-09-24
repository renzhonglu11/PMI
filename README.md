# Project 1 — traffic-light controller (receiver)

This branch runs the traffic-light state machine on a **NUCLEO-L053R8**. It writes LED patterns to an **MCP23017** GPIO expander at I2C address `0x20`, initializes an **ILI9341** display, and reads two buttons on **PB1/PB2**. The state definitions and transition table are in [`include/mcp23017.h`](include/mcp23017.h) and [`src/mcp23017/mcp23017.c`](src/mcp23017/mcp23017.c).

## UART receiver and timeout

`src/main.c` initializes USART2 at **115200 baud** with receive interrupts and starts TIM2. Receiving a byte clears `uart_timeout_flag` and echoes the byte. When the timer has counted three update events without a received byte, the main loop displays the lost-connection LED pattern; otherwise it runs the state machine. The code currently does **not** map received traffic-light characters to state transitions. The elapsed timeout in seconds depends on the timer clock and the `PSC`/`ARR` settings in [`src/uart_irq/uart_irq.c`](src/uart_irq/uart_irq.c).

The companion sender is on [`proj_1_uart_master`](https://github.com/renzhonglu11/PMI/tree/proj_1_uart_master). For a host-side serial demonstration, `test-main-control.py` and `test-slave-control.py` require `pyserial`; set each script's hard-coded serial port before running it.

## Build and upload

```sh
pio run
pio run --target upload
```

The PlatformIO environment is `nucleo_l053r8` using CMSIS. Connect the MCP23017, display and buttons according to the GPIO/I2C setup in the source before running on hardware.
