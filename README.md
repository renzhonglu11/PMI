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
