# Ampelsteuerung

## MCP

Base address: `0x20`

## GPIO

- GPIOA
- GPIOB

## I2C

Address Register: `Device address` + `Register address`

## Datastructure of the finite state machine (Traffic lights)

Almost all the data structure can be found in `mcp23017.h`.
FSM table can be found in `mcp23017.c`.

## TODO

- [x] interrupt
- [x] GPIO
- [x] button
- [x] LED
- [x] loop LED
- [x] traffic light
- [x] led display for traffic light
- [ ] connect two I2C boards (if time allows)

## Details of exercise 3

We use UART interrupt to receive the data from another board. The Timeout mechanism is implemented by using timer interrupt. For each 3 seconds of lost connection of UART, the timeout will be triggered.

### Note

Until now only not all functions are implemented.  Only the slave board has the timeout mechanism. After the connection of slave board, the leds will enter the initial state of exercise2. If there is timeout of receiving data, the leds will then enter to the lost connection state.

Master boards can now only send characters to slave board. No timeout mechanism codes are implemented until yet.

### TIMER2 configuration

```
TIM2->PSC = 639999;
TIM2->ARR = 500;
```
Freuency of timer: `64MHZ`