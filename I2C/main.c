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
#define LCD_Addres 0x40
#define LED PORTBbits.RB0

 
void I2C_init();
void I2C_start();
void I2C_stop();
void I2C_write(unsigned char Write_data);
void Lcd_init();

void Lcd_Send(unsigned char Send_data);
void Lcd_Cmd(unsigned char cmd);
void Lcd_data(unsigned char data);
void Lcd_string(const unsigned char *str, unsigned char num);
void Lcd_SetCursor(unsigned char Row, unsigned char Column);
int Int_String(int num, char *buffer);

int count, loop;
char buffer[20];

void main(void) {
    TRISB0 = 0;
    LED = 0;
    
    I2C_init();
    Lcd_init();
    __delay_ms(1);
    Lcd_SetCursor(1, 6);
    __delay_ms(1);
    Lcd_string("Hello", 5);
    __delay_ms(1);
    Lcd_SetCursor(2, 1);
    __delay_ms(1);
    Lcd_string("Count : ", 8);
    
    while(1){
        LED = 1;
        __delay_ms(1000);
        LED = 0;
        __delay_ms(1000);
        Lcd_SetCursor(2, 9);
        __delay_us(50);
        count++;
        loop = Int_String(count, buffer);
        for(int x=0; x<loop; x++){
            __delay_us(50);
            Lcd_data(buffer[x]);
            
        }
    }
    return;
}

void I2C_init(){
    SSPCON = 0x28;
    SSPSTATbits.SMP = 1;
    SSPADD = 49;
    TRISC3 = 1;
    TRISC4 = 1;
}

void I2C_start(){
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
    PIR1bits.SSPIF = 0;
}

void I2C_stop(){
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
    PIR1bits.SSPIF = 0;
}

void I2C_write(unsigned char Write_data){
    SSPBUF = Write_data;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}

void Lcd_init() {
    __delay_ms(50);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x28);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x01);
}

void Lcd_Send(unsigned char Send_data){
    Send_data |= 0x08;
    I2C_write(Send_data | 0x04);
    __delay_us(50);
    I2C_write(Send_data & ~0x04);
    __delay_us(50);
}

void Lcd_Cmd(unsigned char cmd){
    unsigned char msb = cmd & 0xF0;
    unsigned char lsb = (cmd << 4) & 0xF0;
    
    I2C_start();
    I2C_write(LCD_Addres);
    Lcd_Send(msb);
    Lcd_Send(lsb);
    I2C_stop();
}

void Lcd_data(unsigned char data){
    unsigned char msb = (data & 0xF0) | 0x01;
    unsigned char lsb = ((data << 4) & 0xF0) | 0x01;
    
    I2C_start();
    I2C_write(LCD_Addres);
    Lcd_Send(msb);
    Lcd_Send(lsb);
    I2C_stop();
}

void Lcd_string(const unsigned char *str, unsigned char length){
    unsigned char i;
    for(i=0; i<length; i++){
        Lcd_data(str[i]);
        __delay_us(50);
    }
}

void Lcd_SetCursor(unsigned char Row, unsigned char Column){
    unsigned char Pos;
    if(Row == 1){
        Pos = (0x80 + (Column - 1));
    }
    else{
        Pos = (0xC0 + (Column - 1));
    }
    Lcd_Cmd(Pos);
}

int Int_String(int num, char *buffer){
    int dig = 0, end;
    int start;
    
    while(num>0){
        buffer[dig++] = '0' + (num % 10);
        num /= 10;
    }
    
    buffer[dig] = '\0';
    
    end = dig - 1;
    start = 0;
    
    while(start < end){
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    return dig;
}

