#include "ADC-7seg.h"


void ADC(void) {
    if(ADCON0bits.CHS == 8){                        // Si el channel es 1 (puerto AN1)
        unidades = ADRESH;                          // Valor analógico traducido = unidades
        for(int i = 0; i<3 ; i++){                  // De i = 0 hasta i = 2
            dig[i] = unidades % 16;                 // array[i] = cont_vol mod 16(retornar residuo). Devuelve digito por dígito de un número hexadecimal.
            unidades = (unidades - dig[i])/16;      // unidades = valor sin último digito.
        }
    }
    else{
            
    }                                               //   entonces asignar PORTC = ADRESH
    PIR1bits.ADIF = 0;                              // Limpiar bander de interrupción ADC
    
}


int tabla(int a){
    switch (a){                                     // Ingresar valor de "a" a switch case
        case 0:                                     // Si a = 0
            return 0b00111111;                      // devolver valor 0b00111111
            break;
        case 1:                                     // Si a = 1
            return 0b00000110;                      // devolver valor 0b00000110 
            break;
        case 2:                                     // Si a = 2
            return 0b01011011;                      // devolver valor 0b01011011
            break;
        case 3:                                     // Si a = 3
            return 0b01001111;                      // devolver valor 0b01001111
            break;
        case 4:                                     // Si a = 4
            return 0b01100110;                      // devolver valor 0b01100110
            break;
        case 5:                                     // Si a = 5
            return 0b01101101;                      // devolver valor 0b01101101
            break;
        case 6:                                     // Si a = 6
            return 0b01111101;                      // devolver valor 0b01111101
            break;
        case 7:                                     // Si a = 7
            return 0b00000111;                      // devolver valor 0b01111101
            break;
        case 8:                                     // Si a = 8
            return 0b01111111;                      // devolver valor 0b01111111
            break;
        case 9:                                     // Si a = 9
            return 0b01101111;                      // devolver valor 0b01101111
            break;
        case 10:                                    // Si a = 10 (letra A)
            return 0b01110111;                      // devolver valor 0b01110111
            break;
        case 11:                                    // Si a = 11 (letra B)
            return 0b01111100;                      // devolver valor 0b01111100
            break;
        case 12:                                    // Si a = 12 (letra C)
            return 0b00111001;                      // devolver valor 0b00111001
            break;
        case 13:                                    // Si a = 13 (letra D)
            return 0b01011110;                      // devolver valor 0b01011110
            break;
        case 14:                                    // Si a = 14 (letra E)
            return 0b01111001;                      // devolver valor 0b01111001
            break;
        case 15:                                    // Si a = 15 (letra F)
            return 0b01110001;                      // devolver valor 0b01110001
            break;
        default:
            break;
            
    }
}

