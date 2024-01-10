# Project2 of PMI


## ADC configuration

- RCC_APBR2ENR -> ADCEN
- RCC_IOPENR: open clock of GPIO pin
- prescale?
- GPIO analog input?
- 


### ADC registers

ADC1->CR:
ADEN: check if ADC is enable
ADSTART: check if ADC starts


ADC1->ISR: status register
EOCAL: bit is set by hardware when calibration completes