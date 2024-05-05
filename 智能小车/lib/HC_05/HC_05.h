#ifndef __HC_05_H
#define __HC_05_H

extern uint8_t Word_exchange[];

void HC_05_SelfTest(void);
void HC_05_Command_Enter(void);
void HC_05_Command(uint8_t * Command);
void HC_05_Command_Exit(void);
void HC_05_ReceiveIT(void);
void HC_05_Receive(void);

#endif

