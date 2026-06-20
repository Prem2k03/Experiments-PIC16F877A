#include <xc.h>

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000

void tx(unsigned char data){
    while(!PIR1bits.TXIF);
    if(PIR1bits.TXIF){
        TXREG = data;
        PIR1bits.TXIF = 0;
    }
}

void string(const unsigned char *str, unsigned char num){
    unsigned char i;
    for(i=0; i<num; i++){
        tx(str[i]);
        __delay_ms(500);
    }
}
void main(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 0x81;
    __delay_ms(100);
    string("FROM PIC Tx : ", 14);
    __delay_ms(2000);
    tx('H');
    __delay_ms(1000);
    tx('i');
    
    return;
}
