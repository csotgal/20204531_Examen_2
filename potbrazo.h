#ifndef POTBRAZO_H 
#define POTBRAZO_H

#include <stdint.h>
#include "stm32f10x.h"

void configurarADC(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t leerPuerto(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

#endif