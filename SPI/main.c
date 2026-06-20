#include <xc.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

#define CS  RC2          // pick any free pin, e.g. RC2, just not RC3/4/5
#define CS_LOW()  (CS = 0)
#define CS_HIGH() (CS = 1)


void SPI_Initialize_Master(void)
{
    TRISC3 = 0;     // SCK output
    TRISC4 = 1;     // SDI input  <-- was missing
    TRISC5 = 0;     // SDO output
    TRISC2 = 0;     // CS output
    CS_HIGH();      // idle high

    SSPSTAT = 0b00000000;  // SMP=0, CKE=0 -> Mode 0,0 with CKP=0 below
    SSPCON  = 0b00100000;  // SSPEN=1, CKP=0, SSPM=0000 (Master, Fosc/4)
}

char SPI_Write(char data)
{
    SSPBUF = data;
    while (!SSPSTATbits.BF);   // wait until byte fully shifted
}
unsigned char SPI_Read(){
    SSPBUF = 0x00;
    while(!SSPIF);
    return (SSPBUF);
}

void EEPROM_Write(unsigned char msb, unsigned char lsb, unsigned char data)
{
    CS_LOW();
    SPI_Write(0x02);
    SPI_Write(msb);
    SPI_Write(lsb);
    SPI_Write(data);
    CS_HIGH();
}

unsigned char EEPROM_Read(unsigned char msb, unsigned char lsb)
{
    CS_LOW();
    SPI_Write(0x03);          // READ opcode, not 0x02
    SPI_Write(msb);
    SPI_Write(lsb);
    unsigned char status = SPI_Read();
    CS_HIGH();
    return status;
}



void main(void)
{
    SPI_Initialize_Master();      // <-- this call was missing
    
    CS_LOW();
    SPI_Write(0x06);
    CS_HIGH();
    __delay_ms(100);
    
    EEPROM_Write(0x03, 0x43, 0x09);
    __delay_ms(10);
    
    CS_LOW();
    SPI_Write(0x04);
    CS_HIGH();
    __delay_ms(100);
    
    unsigned char value = EEPROM_Read(0x03, 0x43);

    while(1);
}