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

unsigned int raw_adc_data, order;
int j;
int value[4];

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
    __delay_ms(2);
}

void lcd_string(const unsigned char *str, unsigned char num){
    unsigned char i;
    for(i=0; i<num; i++){
        lcd_data(str[i]);
        __delay_ms(2);
    }
}

void lcd_clear(){
    lcd_cmd(0x01);
}

void adc_init(){
    ADCON0 = 0x41;
    ADCON1 = 0x8E;
}

unsigned int adc_read(){
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    return ((ADRESH << 8) + ADRESL);
}

void main(void) {
    TRISB = 0x00;
    TRISCbits.TRISC0 = 0; TRISCbits.TRISC1 = 0; TRISCbits.TRISC2 = 0;
    
    lcd_init();
    adc_init();
    
    lcd_cmd(0x80);
    lcd_string("POT Value : ", 12);
    
    while(1){
        
        lcd_cmd(0xC0);
        lcd_string("     ", 5);
        
        raw_adc_data = adc_read();
        unsigned int temp = raw_adc_data;
        
        for(order = 0; temp > 0; order++){
            value[order] = temp % 10;
            temp = temp / 10;
            
        }
        lcd_cmd(0xC0);
        for(j=order - 1; j>=0; j--){
            lcd_data(value[j] + '0');
        }
        __delay_ms(100);
    }
    return;
}
