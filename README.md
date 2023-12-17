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

## I2C

The software based I2C is configured as open-drain and pull-up.

Open-drain allows device on the bus to pull the line low to trainsmit a '0' and then release the line (let it float) for the pull-up resistor to pull it high, represeting a '1'.

### Why open-drain?

1. Multi-Master capability
2. Bidirectional data transfer on a single line
3. Simplicity and reliability
4. There's no risk of one device trying to drive the line high while another tries to drive it low. Such a situation, if it were to occur, would create a direct path from the positive voltage supply to ground, resulting in a short circuit.


### Notes of I2C:

**Simultaneous Actions**: If one device pulls the line low (to send a '0') while another device releases the line (either to send a '1' or to stop communicating), the action of pulling the line low will dominate. This is because the low state is an active state, where the line is physically connected to ground, whereas the high state is passive, relying only on the pull-up resistor.
