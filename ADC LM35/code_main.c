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

unsigned int raw_adc_data, once, tens, m_once, m_tens, a, b;
float temp, processed_adc_data;

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
    lcd_cmd(0x80);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_string(const unsigned char *str, unsigned char num){
    unsigned char i;
    for(i=0; i<num; i++){
        lcd_data(str[i]);
        __delay_ms(100);
    }
}

void lcd_clear(){
    lcd_cmd(0x01);
    __delay_ms(2);
}

void adc_init(){
    ADCON0 = 0x41;
    ADCON1 = 0x8D;
}

void interrupt adc(){
    if(PIR1bits.ADIF == 1){
        raw_adc_data = (ADRESH<<8);
        raw_adc_data = raw_adc_data + ADRESL;
        PIR1bits.ADIF = 0;
    }
}

void main(void) {
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    
    TRISB = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    adc_init();
    lcd_init();
    
    lcd_string("Sensor : ", 9);
    
    while(1){
        
        ADCON0 = ADCON0 | (0x04);
        temp = raw_adc_data/2.046; // Example: if the temp = 10.23
        
        processed_adc_data = temp * 100; // now processed_adc_data = 1023 
        
        a = (int)processed_adc_data / 10; // a = 102
        m_tens = (int)processed_adc_data % 10; // m_tens = 3
        
        b = a / 10; // b = 10
        m_once = a % 10; // m_once = 2
        
        tens = b / 10; // tens = 1
        once = b % 10; // once = 0
        /* So : tens    once    .   m_once  m_tens
                 1        0     .       2     3    */
        lcd_cmd(0x89);
        lcd_data(tens + 0x30);
        lcd_data(once + 0x30);
        lcd_data('.');
        lcd_data(m_once + 0x30);
        lcd_data(m_tens + 0x30);
        __delay_ms(100);
    }
    return;
}










#include <xc.h>

// CONFIG
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

unsigned int raw_adc_data;
float temp;

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

void adc_init(){
    ADCON0 = 0x41;   // Channel 0, ADC ON
    ADCON1 = 0x80;   // Right justified, Vref = Vdd
}

unsigned int adc_read(unsigned char channel){
    ADCON0 &= 0xC5;
    ADCON0 |= channel<<3;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<8) + ADRESL);
}

void main(void) {
    TRISB = 0x00;
    TRISC = 0x00;
    adc_init();
    lcd_init();
    lcd_string("Sensor:", 7);

    while(1){
        raw_adc_data = adc_read(0);
        temp = (raw_adc_data * 500.0) / 1023.0; // LM35 °C

        unsigned int temp_int = (unsigned int)temp;
        unsigned int temp_dec = (unsigned int)((temp - temp_int) * 10);

        lcd_cmd(0x89);
        lcd_data((temp_int / 10) + '0');
        lcd_data((temp_int % 10) + '0');
        lcd_data('.');
        lcd_data(temp_dec + '0');
        lcd_data('C');

        __delay_ms(500);
    }
}
