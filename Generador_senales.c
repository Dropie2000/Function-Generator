#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "i2c.h"
#include "mcp4725.h"

char dato;          //Cadena en donde se guardará el dato recibido
int ascii=48;          //Se guarda el valor ascii en una valiable de tipo entero
int res;            //Variable que guarda el valor de la resolución de la señal
int ampl;           //Variable que guarda el valor de la amplitud de la señal
int vsin;          //Varible que guarda el valor del seno
int n;              //Se usará para el conteo de las señales
int t;              //Se usará para el conteo en la señal triangular
int const dl=1;    //Para practica
//int const del=10;   //Para simulacion

void sinu(int amplitud,int resolucion);
void triang(int amplitud2, int resolucion2);
void sierra(int amplitud3, int resolucion3);

void main(void) {
    ANSEL=0;
    I2C_Init_Master(I2C_100KHZ);                    // Inicializa el protocolo i2c
    DAC_Init(MCP4725_ADDRESS);                      // Inicializa el DAC MCP4725
    SPBRGH=0x02;        //n=520 para un baud rate de 9600
    SPBRG=0x08;
    BAUDCTLbits.BRG16=1;    //Configurar baud rate de 16 bits
    TXSTAbits.BRGH=1;       //Configurar baud rate de alta velocidad
   //Con esta configuracion, el baud rate se obtiene con: Baud=Fosc/(4(n+1))
    //En donde n es el valor en el registro SPRBG y SPRBGH
    //Resolviendo para n, se tiene: n=(Fosc/(4*Baud rate))-1, este valor es el 
    //primero en configurarse para evitar fallos, para un baud rate de 9600, n=520
    TXSTAbits.SYNC=0;       //Configurar modulo eusart en modo asincrono
    RCSTAbits.SPEN=1;       //Habilitar el puerto serial
    INTCON=0b11000000;        //Habilitacion de interrupciones globales y de perifericos
    PIE1bits.RCIE=1;        //Habilitar la interrupcion por puerto serial
    RCSTAbits.CREN=1;       //Habilitar el receptor serial
    TXSTAbits.TXEN=1;       //Habilitar la transmision
    while (1) {
        switch (ascii)
        {
            case 48:        //Tecla 0
                DAC_Write_Value(MCP4725_ADDRESS, 1500);
                __delay_ms(dl);
                DAC_Write_Value(MCP4725_ADDRESS, 3500);
                __delay_ms(dl);
                DAC_Write_Value(MCP4725_ADDRESS, 500);
                __delay_ms(dl);
                DAC_Write_Value(MCP4725_ADDRESS, 4000);
                __delay_ms(dl);
                break;
            case 49:        //Tecla 1
                sinu(512,32);
                break;
            case 50:        //Tecla 2
                sinu(1024,64);
                break;
            case 51:        //Tecla 3
                sinu(2000,128);
                break;
            case 52:        //Tecla 4
                triang(4000,200);
                break;
            case 53:        //Tecla 5
                triang(2000,80);
                break;
            case 54:        //Tecla 6
                triang(1000,50);
                break;
            case 55:        //Tecla 7
                sierra(4095,256);
                break;
            case 56:        //Tecla 8
                sierra(2048,128);
                break;
            case 57:        //Tecla 9
                sierra(1000,32);
                break;
            default:
            break;
        }   
    }
}


void __interrupt() int_usart()      //Función de lectura del caracter recibido
{
    if(PIR1bits.RCIF)
    {
        dato=RCREG;
        ascii=dato;
        TXREG=dato;        //Escritura de los datos recibidos
        while(TXSTAbits.TRMT==0)
            ;
        TXREG=13;               //Retorno de carro
        while(TXSTAbits.TRMT==0)
            ;
        TXREG=10;               //Salto de linea
        while(TXSTAbits.TRMT==0)
            ;
    }
}

void sinu(int amplitud,int resolucion)
{
    while(n<=4095)
    {
    vsin=amplitud*sin(n*2*3.1416/4095)+amplitud;
    DAC_Write_Value(MCP4725_ADDRESS, vsin);
    n=n+resolucion;   
    __delay_ms(dl);
    }
    n=0;
    __delay_ms(dl);
}
void triang(int amplitud2, int resolucion2)
{
    while(n<=amplitud2)
    {
        DAC_Write_Value(MCP4725_ADDRESS, t);
        __delay_ms(dl);
        if(n<amplitud2/2)
        {
            t=t+resolucion2;
        }
        else
        {
            t=t-resolucion2;
        }
        n=n+resolucion2;
    }
    n=0;
    t=0;
}
void sierra(int amplitud3, int resolucion3)
{
    while(n<=amplitud3)
    {
    DAC_Write_Value(MCP4725_ADDRESS, n);
    n=n+resolucion3; 
    __delay_ms(dl);
    }
    n=0;
}