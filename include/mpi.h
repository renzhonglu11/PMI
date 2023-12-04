#ifndef MPI_H
#define MPI_H

#include <pmi_stddefs.h>


 int32_t spi_init_adxl345 ( void );
 int32_t spi_txrx ( uint8_t * buf , uint32_t size );


#endif /* MPI_H */