#include "global_common.h"

/*Initialize global variables*/
uint16_t trigger_index = -1;
volatile uint8_t data_ready = 0;
uint8_t rc_range = 0;
uint32_t p2p_val = 0;
volatile uint8_t graph_ready = 0;
const float DIVISOR_MV = 4.095;    // divisor for converting ADC value to mV, 4095/1000
volatile uint8_t zoom_lvl = 1;
uint16_t final_time_period = 0;



/**
 * Calculates the power of 2 for a given exponent.
 *
 * @param exponent The exponent value.
 * @return The result of 2 raised to the power of the exponent.
 */
uint8_t power_of_2(uint8_t exponent)
{
  uint8_t r = 1;

  for (uint8_t i = 1; i < exponent; i++)
  {
    r *= 2;
  }

  return r;
}

/**
 * @brief Extracts samples from the ADC buffer and stores them in the provided array.
 *
 * This function extracts samples from the ADC buffer and stores them in the provided array.
 * The number of samples extracted is equal to the size of the buffer(240).
 *
 * @param extracted_data Pointer to the array where the extracted samples will be stored.
 * @return Returns RC_SUCC to indicate successful extraction.
 */
uint32_t extract_samples(uint16_t *extracted_data)
{
  uint8_t end_index = (trigger_index + POST_TRIGGER_COUNT) % BUFFER_SIZE;
  uint8_t start_index = end_index;
  int extracted_index = 0;

  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    extracted_data[extracted_index++] = adc_buffer[start_index];
    start_index = (start_index + 1) % BUFFER_SIZE;
  }

  return RC_SUCC;
}
