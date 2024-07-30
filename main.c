#include "stm32f10x.h"
#include "potbrazo.h"
#include "servobrazo.h"
#include "uartbrazo.h"

#define NUM_POSICIONES 10

uint16_t posiciones[NUM_POSICIONES][4] = {{6000}};
uint8_t numPosicionesGuardadas = 0;

int main(void) {
    // Configura los pines de los potenciómetros como entradas analógicas
    configurarADC(GPIOA, GPIO_Pin_0);
    configurarADC(GPIOA, GPIO_Pin_1);
    configurarADC(GPIOA, GPIO_Pin_2);
    configurarADC(GPIOA, GPIO_Pin_3);

    // Configura los timers para generar PWM en los pines de los servos
    configurarTimerPWM();

    // Configura la comunicación UART
    configurarUART(USART3);

    // Mensaje de bienvenida
    enviarPalabra(USART3, "BIENVENIDOS AL PROGRAMA BRAZO ROBOTICO\n");

    while (1) {
        // Verifica si hay un carácter disponible en la USART
        if (USART3->SR & USART_SR_RXNE) { 
            char modo = leerCaracter(USART3);

            switch (modo) {
                case 'a':
                    // Brazo robótico inhabilitado
                    break;
                case 'b':
                    // Teleoperación
                    while (leerCaracter(USART3) != 'c') {
                        for (int i = 0; i < 4; i++) {
                            uint16_t valorADC = leerPuerto(ADC1, i);
                            cambiarAngulo(i + 1, valorADC); // Cambiar ángulo del servo correspondiente
                        }
                    }
                    break;
                case 'g':
                    // Guarda posición
                    if (numPosicionesGuardadas < NUM_POSICIONES) {
                        for (int i = 0; i < 4; i++) {
                            posiciones[numPosicionesGuardadas][i] = leerPuerto(ADC1, i);
                        }
                        numPosicionesGuardadas++;
                    } else {
                        enviarPalabra(USART3, "ARREGLO LLENO\n");
                    }
                    break;
                case 'h':
                    // Reproduce posiciones guardadas
                    for (int i = 0; i < numPosicionesGuardadas; i++) {
                        for (int j = 0; j < 4; j++) {
                            cambiarAngulo(j + 1, posiciones[i][j]);
                        }
                        for (int k = 0; k < 1000000; k++); // Retardo
                    }
                    break;
                case 'i':
                    // Reinicia arreglo de posiciones
                    for (int i = 0; i < NUM_POSICIONES; i++) {
                        for (int j = 0; j < 4; j++) {
                            posiciones[i][j] = 6000;
                        }
                    }
                    numPosicionesGuardadas = 0;
                    break;
            }
        }
    }
}
