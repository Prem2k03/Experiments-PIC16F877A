#include <xc.h>

/* ================= CONFIGURATION ================= */
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

/* ================= LCD PIN DEFINITIONS ================= */
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

/* ================= GLOBAL VARIABLES ================= */
volatile unsigned char rx_data;
volatile unsigned char rx_flag = 0;

/* ================= LCD FUNCTIONS ================= */

void lcd_pulse(void) {
    EN = 1;
    __delay_us(2);
    EN = 0;
    __delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
    PORTB = cmd;
    RS = 0;
    RW = 0;
    lcd_pulse();
}

void lcd_data(unsigned char data) {
    PORTB = data;
    RS = 1;
    RW = 0;
    lcd_pulse();
}

void lcd_init(void) {
    __delay_ms(20);      // Power-up delay

    lcd_cmd(0x38);       // 8-bit, 2-line
    lcd_cmd(0x0C);       // Display ON, cursor OFF
    lcd_cmd(0x01);       // Clear display
    lcd_cmd(0x06);       // Entry mode
}

/* ================= UART INTERRUPT ================= */

void __interrupt() uart_rx_isr(void) {
    if (PIR1bits.RCIF) {
        rx_data = RCREG;     // Read received byte
        rx_flag = 1;

        if (RCSTAbits.OERR) {  // Overrun error handling
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
    }
}

/* ================= MAIN ================= */

void main(void) {

    /* I/O CONFIG */
    TRISB = 0x00;        // LCD data
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;

    /* LCD INIT */
    lcd_init();

    /* UART CONFIG (9600 baud @ 20MHz) */
    TXSTA = 0x24;        // BRGH=1, TXEN=1
    RCSTA = 0x90;        // SPEN=1, CREN=1
    SPBRG = 129;         // 9600 baud

    /* INTERRUPTS */
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE  = 1;

    /* MAIN LOOP */
    while (1) {
        if (rx_flag) {
            lcd_data(rx_data);   // Print received ASCII
            rx_flag = 0;
        }
    }
}
