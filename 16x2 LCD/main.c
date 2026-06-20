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
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

void lcd_data(unsigned char data){
    PORTB = data;
    EN = 1;
    RS = 1;
    RW = 0;
    __delay_ms(2);
    EN = 0;
}

void lcd_cmd(unsigned char cmd){
    PORTB = cmd;
    EN = 1;
    RS = 0;
    RW = 0;
    __delay_ms(2);
    EN = 0;
}

void init(){
    lcd_cmd(0x38);
    lcd_cmd(0x80);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
}

void lcd_string(const unsigned char *str, unsigned char num){
    unsigned char i;
    for(i=0; i<num; i++){
        lcd_data(str[i]);
        __delay_ms(10);
    }
}

void main(void) {
    TRISB = 0x00;
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    
    init();
    
    lcd_string("Hello Prem", 10);
    lcd_cmd(0xC0);
    lcd_string("Moving >>", 9);
    __delay_ms(5000);
    while(1){
        lcd_cmd(0x1C);
        __delay_ms(100);
    }
    return;
}
