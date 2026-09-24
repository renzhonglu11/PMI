# Project 1 — UART sender (master board)

This branch is the companion to the [Project 1 receiver](https://github.com/renzhonglu11/PMI/tree/proj_1). It initializes the NUCLEO-L053R8, MCP23017 and ILI9341, then sends `Gg\n` through **USART2 at 115200 baud** every two seconds. The actual loop in [`src/main.c`](src/main.c) sends this fixed example string; it does not yet transmit the traffic-light state machine's live state or detect a lost UART connection.

## Build and upload

```sh
pio run
pio run --target upload
```

Connect the boards' USART2 TX/RX lines and a common ground as appropriate for the hardware setup. USART2 uses **PA2 (TX)** and **PA3 (RX)** in [`src/uart_irq/uart_irq.c`](src/uart_irq/uart_irq.c). The Python serial examples `test-main-control.py` and `test-slave-control.py` require `pyserial` and have hard-coded ports that must be changed for your machine.
