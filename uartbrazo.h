#ifndef UARTBRAZO_H
#define UARTBRAZO_H

#include <stdint.h>
#include "stm32f10x.h"

void configurarUART(USART_TypeDef* USARTx);
void enviarCaracter(USART_TypeDef* USARTx, char c);
void enviarPalabra(USART_TypeDef* USARTx, char* palabra);
char leerCaracter(USART_TypeDef* USARTx);
void nuevaLinea(USART_TypeDef* USARTx);

#endif