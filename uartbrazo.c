#include "uartbrazo.h"

void configurarUART(USART_TypeDef* USARTx) {
    // Habilita el reloj de la USART3 y del puerto GPIOB
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Configura los pines PB10 (TX) y PB11 (RX) como función alternativa
    GPIOB->CRH &= ~(0xFF << 8);   // Limpia configuración de PB10 y PB11
    GPIOB->CRH |= 0x0B << 8;      // PB10: TX (modo alternate function push-pull)
    GPIOB->CRH |= 0x04 << 12;     // PB11: RX (modo input floating)

    // Configura los parámetros de la USART
    USARTx->BRR = 72000000 / 115200; // Baud rate 115200
    USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE; // Habilita transmisor y receptor

    // Habilita la USART
    USARTx->CR1 |= USART_CR1_UE;
}

void enviarCaracter(USART_TypeDef* USARTx, char c) {
    while (!(USARTx->SR & USART_SR_TXE)); // Espera a que el registro de transmisión esté vacío
    USARTx->DR = c; // Envia el carácter
}

void enviarPalabra(USART_TypeDef* USARTx, char* palabra) {
    while (*palabra) {
        enviarCaracter(USARTx, *palabra++);
    }
}

char leerCaracter(USART_TypeDef* USARTx) {
    while (!(USARTx->SR & USART_SR_RXNE)); // Espera a que haya un carácter recibido
    return USARTx->DR; // Devuelve el carácter recibido
}

void nuevaLinea(USART_TypeDef* USARTx) {
    enviarCaracter(USARTx, '\r'); // Retorno de carro
    enviarCaracter(USARTx, '\n'); // Nueva línea
}