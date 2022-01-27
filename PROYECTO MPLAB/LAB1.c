/*
 * File:   LAB1.c
 * Author: José Santizo
 *
 * Creado el 24 de enero de 2022
 * 
 * Descripción: Uso de librerias e interrupciones para controlar un puerto, generar contador hexadecimal y alarma visual por LED
 * 
 */

//---------------------Bits de configuración-------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------Librerías utilizadas en código-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 8000000

//-----------------------Constantes----------------------------------
#define  valor_tmr0 156                        // valor_tmr0 = 156 (0.05 ms)

//-----------------------Variables------------------------------------
int unidades = 0;
int decenas = 0;
uint8_t disp_selector = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                               // Función de setup
void limite(void);
void tmr0(void);                                // Función para reiniciar TMR0
void displays(void);                            // Función para alternar valores mostrados en displays

//-------------Funciones que retornan variables-----------------------
int  tabla(int a);                              // Tabla para traducir valores a displays de 7 segmentos

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(PIR1bits.ADIF){
        if(ADCON0bits.CHS == 5){                // Si el channel es 1 (puerto AN1)
            unidades = ADRESH;
        }
        else {
        }
        PIR1bits.ADIF = 0;                      // Limpiar bander de interrupción ADC
    }
    if(T0IF){
        tmr0();                                 // Reiniciar TMR0
        displays();
    }
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();                                    // Subrutina de setup
    ADCON0bits.GO = 1;                          // Comenzar conversión ADC 
    while(1){
        if(ADCON0bits.GO == 0){             // Si bit GO = 0
            if(ADCON0bits.CHS == 6){        // Si Input Channel = AN1
                ADCON0bits.CHS = 5;         // Asignar input Channel = AN0
                __delay_us(50);             // Delay de 50 ms
            }
            else{                           // Si Input Channel = AN0
                ADCON0bits.CHS = 6;         // Asignar Input Channel = AN1
                __delay_us(50);
            }
            __delay_us(50);
            ADCON0bits.GO = 1;              // Asignar bit GO = 1
        } 
        limite();
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0b00100000;                         // Pines 0 y 1 de PORTA como analógicos
    ANSELH = 0;
    
    
    TRISB = 0;                                  // PORTA, bit 0 y 1 como entrada analógica
    TRISA = 0;
    TRISC = 0;                                  // PORTC como salida
    TRISD = 0;                                  // PORTD como salida                           
    TRISE = 0b001;                              // PORTE como salida
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;                                  // Limpiar PORTD
    PORTC = 0;                                  // Limpiar PORTC
    PORTE = 0;                                  // Limpiar PORTE
    
    //Configuración de oscilador
    OSCCONbits.IRCF = 0b0111;                   // Oscilador a 8 MHz = 111
    OSCCONbits.SCS = 1;
    
    //Configuración de TMR0
    OPTION_REGbits.T0CS = 0;                    // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;                    // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 1;                     // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the WDT
    OPTION_REGbits.PS2 = 0;                     // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = valor_tmr0;                          // preset for timer register
    
    //Configuración del ADC
    ADCON1bits.ADFM = 0;                        // Resultado justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                       // Voltaje 0 de referencia = VSS
    ADCON1bits.VCFG1 = 0;                       // Voltaje 1 de referencia = VDD
    
    ADCON0bits.ADCS = 0b10;                     // Conversión ADC generada con FOSC/32
    ADCON0bits.CHS = 5;                    // Input Channel = AN0
    ADCON0bits.ADON = 1;                        // ADC = enabled
    __delay_us(200);
    
    //Configuración de interrupciones
    INTCONbits.T0IF = 0;                        // Habilitada la bandera de TIMER 0      
    INTCONbits.T0IE = 1;                        // Habilitar las interrupciones de TIMER 0
    INTCONbits.GIE = 1;                         // Habilitar interrupciones globales
    PIR1bits.ADIF = 0;                          // Limpiar bandera de interrupción del ADC
    PIE1bits.ADIE = 1;                          // Interrupción ADC = enabled
    INTCONbits.PEIE = 1;                        // Interrupciones periféricas activadas
    
    return;
}

void tmr0(void){
    INTCONbits.T0IF = 0;                        // Limpiar bandera de TIMER 0
    TMR0 = valor_tmr0;                          // TMR0 = 255
    return;
}

void displays(void){
    PORTD = disp_selector;
    if(disp_selector == 0b001){
        PORTC = tabla(unidades);
        disp_selector = 0b010;
    }
    else if(disp_selector == 0b010){
        PORTC = tabla(decenas);
        disp_selector = 0b001;
    }
}

void limite(void){
    if(unidades == 16){
        unidades = 0;
        decenas++;
    }
    if(unidades == -1){
        unidades = 15;
        decenas--;
    }
    if(unidades == -1 && decenas == -1){
        unidades = 15;
        decenas = 15;
    }
    if(decenas == 16){
        unidades = 0;
        decenas = 0;
    }
}

int tabla(int a){
    switch (a){
        case 0:
            return 0b00111111;                  //Número 0
            break;
        case 1:
            return 0b00000110;                  //Número 1
            break;
        case 2:
            return 0b01011011;                  //Número 2
            break;
        case 3:
            return 0b01001111;                  //Número 3
            break;
        case 4:
            return 0b01100110;                  //Número 4
            break;
        case 5:
            return 0b01101101;                  //Número 5
            break;
        case 6:
            return 0b01111101;                  //Número 6
            break;
        case 7:
            return 0b00000111;                  //Número 7
            break;
        case 8:
            return 0b01111111;                  //Número 8
            break;
        case 9:
            return 0b01101111;                  //Número 9
            break;
        case 10:
            return 0b01110111;                  //Letra A
            break;
        case 11:
            return 0b01111100;                  //Letra B
            break;
        case 12:
            return 0b00111001;                  //Letra C
            break;
        case 13:
            return 0b01011110;                  //Letra D
            break;
        case 14:
            return 0b01111001;                  //Letra E
            break;
        case 15:
            return 0b01110001;                  //Letra F
            break;
        default:
            break;
            
    }
}
