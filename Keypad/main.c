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

#define R1 PORTDbits.RD0
#define R2 PORTDbits.RD1
#define R3 PORTDbits.RD2
#define R4 PORTDbits.RD3
#define C1 PORTDbits.RD4
#define C2 PORTDbits.RD5
#define C3 PORTDbits.RD6
#define C4 PORTDbits.RD7

void lcd_data(unsigned char data){
    PORTB = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_cmd(unsigned char cmd){
    PORTB = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_init(){
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void lcd_string(const unsigned char *str, unsigned char num){
    unsigned char i;
    for(i=0; i<num; i++){
        lcd_data(str[i]);
        __delay_ms(10);
    }
}

void keypad(){
    
    C1 = 1;
    C2 = C3 = C4 = 0;
    if(R1 == 1){
        __delay_ms(100);
        lcd_data('1');
    }
    if(R2 == 1){
        __delay_ms(100);
        lcd_data('4');
    }
    if(R3 == 1){
        __delay_ms(100);
        lcd_data('7');
    }
    if(R4 == 1){
        __delay_ms(100);
        lcd_data('*');
    }
    PORTD = 0x00;
    __delay_us(100);
    
    
    C2 = 1;
    C1 = C3 = C4 = 0;
    if(R1 == 1){
        __delay_ms(100);
        lcd_data('2');
    }
    if(R2 == 1){
        __delay_ms(100);
        lcd_data('5');
    }
    if(R3 == 1){
        __delay_ms(100);
        lcd_data('8');
    }
    if(R4 == 1){
        __delay_ms(100);
        lcd_data('0');
    }
    PORTD = 0x00;
    __delay_us(100);
    
    
    C3 = 1;
    C1 = C2 = C4 = 0;
    if(R1 == 1){
        __delay_ms(100);
        lcd_data('3');
    }
    if(R2 == 1){
        __delay_ms(100);
        lcd_data('6');
    }
    if(R3 == 1){
        __delay_ms(100);
        lcd_data('9');
    }
    if(R4 == 1){
        __delay_ms(100);
        lcd_data('#');
    }
    PORTD = 0x00;
    __delay_us(100);
    
    
    C4 = 1;
    C1 = C2 = C3 = 0;
    if(R1 == 1){
        __delay_ms(100);
        lcd_data('A');
    }
    if(R2 == 1){
        __delay_ms(100);
        lcd_data('B');
    }
    if(R3 == 1){
        __delay_ms(100);
        lcd_data('C');
    }
    if(R4 == 1){
        __delay_ms(100);
        lcd_data('D');

    }
    PORTD = 0x00;
    __delay_us(100);
}

void main(void) {
    TRISD = 0x0F;
    TRISB = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    lcd_init();
    
    lcd_string("Keypad :", 8);
    __delay_ms(100);
    lcd_cmd(0xC0);
    
    while(1){
        keypad();
        __delay_ms(100);
        PORTD = 0x00;
    }
    return;
}
