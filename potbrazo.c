#include "potbrazo.h"

void configurarADC(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // Habilita reloj del puerto GPIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    // Configura el pin como entrada analógica
    GPIOx->CRL &= ~(0xF << (GPIO_Pin * 4)); // Limpia configuración previa
    GPIOx->CRL |= GPIO_CRL_CNF0 << (GPIO_Pin * 4); // Modo entrada analógica

    // Habilita el reloj del ADC1
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Configura el ADC
    ADC1->SQR3 = GPIO_Pin; // Configura el canal del ADC (0-15)
    ADC1->SMPR2 |= 0x7 << (GPIO_Pin * 3); // Tiempo de muestreo máximo

    // Habilita el ADC
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t leerPuerto(ADC_TypeDef* ADCx, uint8_t ADC_Channel) {
    // El ADC esté encendido?
    ADCx->CR2 |= ADC_CR2_ADON; 

    // Selecciona el canal
    ADCx->SQR3 = ADC_Channel; 

    // Inicia la conversión
    ADCx->CR2 |= ADC_CR2_ADON; 

    // Espera a que termine la conversión
    while (!(ADCx->SR & ADC_SR_EOC)); 

    // Lee el resultado
    return ADCx->DR; 
}