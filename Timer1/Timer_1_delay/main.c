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
#define LED PORTCbits.RC0

void delay_ms(unsigned int ms){
    unsigned int i;
    for(i=0; i<ms; i++){
        TMR1H = 0xFB;
        TMR1L = 0x1E;
        while(!PIR1bits.TMR1IF);
        PIR1bits.TMR1IF = 0;
    }
}

void main(){
    TRISCbits.TRISC0 = 0;
    LED = 0;
    T1CON = 0x21;
    
    while(1){
        LED = 1;
        delay_ms(1000);
        LED = 0;
        delay_ms(1000);
    }
    return;
}
