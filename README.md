# Project 2 — three-sensor readout

Firmware for the **NUCLEO-L053R8** that reads an **ADXL345** accelerometer over SPI1, a **QMC5883L** magnetometer over software I2C, and a **DS18B20** temperature sensor over one-wire. Readings appear on an **ILI9341** display and on the Nucleo USB serial connection at **115200 baud**. The main loop updates the serial output roughly every 300 ms after each sensor read.

## Connections used by the code

| Device | Interface / pins |
| --- | --- |
| ADXL345 | SPI1 on PA5/PA6/PA7; PA12 is chip select. |
| QMC5883L | Software I2C on PB8 (SCL) and PB9 (SDA), address `0x0D`. |
| DS18B20 | One-wire on PB13. |

The I2C implementation configures PB8/PB9 as open-drain outputs with pull-ups. Check the sensor and display wiring against the source and module requirements before powering the board.

## Build and run

```sh
pio run
pio run --target upload
pio device monitor
```

`src/main.c` calls `init_proj_2()` and repeatedly calls `send_sensor_data_over_UART()`. The line format is `ACC: X=... Y=... Z=... | MAG: X=... Y=... Z=... | TEMP: ...C`; the same values are drawn on the LCD. Initialization retries if the DS18B20 is not detected. See [`src/my_utils/my_utils.c`](src/my_utils/my_utils.c) for formatting and [Solutions.md](Solutions.md) for additional project notes.
