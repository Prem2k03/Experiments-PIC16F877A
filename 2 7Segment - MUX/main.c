#include <xc.h>
#define _XTAL_FREQ 20000000
#define Seg_ON 0
#define Seg_OFF 1

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void main(void) {
    unsigned char hex[] = {0xBF, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    int i, j, count = 0;
    
    TRISB = 0x00;
    TRISC0 = 0;
    TRISC1 = 0;
    
    while(1){
        
        for(i=0; i<10; i++){
            
            for(j=0; j<10; j++){
                
                while(count <= 1000){
                    
                    PORTCbits.RC0 = Seg_ON;PORTCbits.RC1 = Seg_OFF;
                    PORTB = hex[i];
                    __delay_ms(10);

                    
                    PORTCbits.RC0 = Seg_OFF;PORTCbits.RC1 = Seg_ON;
                    PORTB = hex[j];
                    __delay_ms(10);
                    
                    count += 20;
                }
                count = 0;
            }
        }
    }
}
