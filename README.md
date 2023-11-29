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

## Details to exercise 3


We use UART interrupt to receive data from another board. Timeout is implemented with the help of timer interupt.

Until now, only limited functions are implemented. Once the slave board is connected, the leds will start with the initial state of exercise 2. After 3 secdond timeout, the leds go to the lost connection state. 

**Please Note:**
Only the slave board can detect the timeout when UART connection is lost. The master board can only send the example characters to slave board.
