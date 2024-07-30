#include "servobrazo.h"

// Configuracion para 50Hz en los timers
#define PWM_FREQ 50
#define CPU_FREQ 72000000
#define ARR_VALUE (CPU_FREQ / PWM_FREQ) - 1
#define PSC_VALUE 71 // Para una frecuencia de 50Hz

void configurarTimerPWM(void) {
    // Configuración de los timers TIM1 y TIM3 para generar PWM a 50Hz
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Configura el periodo (ARR) y el preescalador (PSC) de ambos timers
    TIM1->ARR = ARR_VALUE;
    TIM1->PSC = PSC_VALUE;
    TIM3->ARR = ARR_VALUE;
    TIM3->PSC = PSC_VALUE;

    // Configura los canales PWM correspondientes a cada servo
    // TIM1:
    TIM1->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos) | (6 << TIM_CCMR1_OC3M_Pos); // Canales 2 y 3 en modo PWM1
    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC3E; // Habilita salidas de los canales 2 y 3

    // TIM3:
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos); // Canales 1 y 2 en modo PWM1
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E; // Habilita salidas de los canales 1 y 2

    // Habilita los timers
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void cambiarAngulo(uint8_t servo, uint16_t valorADC) {
    // Mapeo del valor ADC (0-4095) a ciclo de trabajo (1ms-2ms)
    uint16_t CCR = 1000 + (valorADC * 1000) / 4095;

    // Limita el valor de CCR al rango válido (1000-2000)
    CCR = (CCR < 1000) ? 1000 : (CCR > 2000 ? 2000 : CCR);

    // Establecer el valor CCR en el registro correspondiente al servo
    switch (servo) {
        case 1: TIM3->CCR2 = CCR; break; // PB1 (TIM3_CH2)
        case 2: TIM3->CCR1 = CCR; break; // PB0 (TIM3_CH1)
        case 3: TIM1->CCR3 = CCR; break; // PA7 (TIM1_CH3)
        case 4: TIM1->CCR2 = CCR; break; // PA6 (TIM1_CH2)
    }
}