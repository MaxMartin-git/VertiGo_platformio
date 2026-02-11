#ifndef UART_LINE_RX_H
#define UART_LINE_RX_H

#include <stdint.h>

void uartLineRxInit();
void uartLineRxPoll();

// Getter für aktuellen Line Error-Wert
int getLineError();

// Check ob UART-Verbindung aktiv ist
bool isUartConnected();

#endif
