#include <xc.h>
#include <stdint.h>

// CONFIG
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

void lcd_cmd(unsigned char cmd){
    PORTB = (cmd & 0xF0);
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_us(10);
    EN = 0;

    PORTB = ((cmd << 4) & 0xF0);
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_us(10);
    EN = 0;

    __delay_ms(2);
}

void lcd_data(unsigned char data){
    PORTB = (data & 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_us(10);
    EN = 0;

    PORTB = ((data << 4) & 0xF0);
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_us(10);
    EN = 0;

    __delay_ms(2);
}

void init(){
    __delay_ms(20);
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    __delay_ms(2);
}

void main(void){
    TRISB = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

    unsigned char words[] = {'H', 'E', 'L', 'L', 'O'};

    init();

    for(int i = 0; i < 5; i++){
        lcd_data(words[i]);
    }

    while(1);  // infinite loop
}
