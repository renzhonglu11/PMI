# Project2 of PMI

## SPI




## ADXL345
(if something does not work, refer back to the datasheet)
### Data Registers

According to the ADXL345 datasheet, the data for each axis is stored in the following registers:

- **X-Axis Data:** `DATAX0` (least significant byte) and `DATAX1` (most significant byte)
- **Y-Axis Data:** `DATAY0` and `DATAY1`
- **Z-Axis Data:** `DATAZ0` and `DATAZ1`

### Read Operation

To read from these registers, you need to set the MSB (most significant bit) of the address to `1`, indicating a read operation.

### Multi-byte Read

You can use multi-byte read capability to read the 6 bytes of the X, Y, and Z data in a single SPI transaction. This is efficient and ensures that the data is read from the same sample.



## TODO
