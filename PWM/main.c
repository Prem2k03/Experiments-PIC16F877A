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

void PWM_DC(unsigned int Percentage){
    unsigned int PWM_Duty = (unsigned int)(4*((int)PR2 + 1)*Percentage/100); // formula for 10bit PWM value
    CCPR1L = PWM_Duty >> 2;
    CCP1CON = CCP1CON & 0xCF; // 1100 1111 Use AND logic except CCPxX and CCPyY (or) clearing this two bits
    CCP1CON = CCP1CON | (0x30 & (PWM_Duty<<4));
}

void main(void) {
    
    TRISCbits.TRISC2 = 0;
    T2CON = 0x05;
    CCP1CON = 0x0C;
    PR2 = 0xF9;
    unsigned int i, j;
    
    while(1){
        
        for(i=1; i<=100; i++){
            PWM_DC(i);
            __delay_ms(10);
        }
        
        for(j=i; j>0; j--){
            PWM_DC(j);
            __delay_ms(10);
        }
    }
    
    return;
}
