#ifndef SERVOBRAZO_H
#define SERVOBRAZO_H

#include <stdint.h>
#include "stm32f10x.h"

void configurarTimerPWM(TIM_TypeDef* TIMx, uint16_t ARR, uint16_t PSC);
void cambiarAngulo(TIM_TypeDef* TIMx, uint8_t Channel, uint16_t valorADC);

#endif