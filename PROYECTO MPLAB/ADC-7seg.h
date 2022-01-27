/* 
 * File: ADC-7seg.h   
 * Author: José Santizo (20185)
 * Comments: Libreria que incluye la configuración de ADC y la traducción para display de 7 segmentos
 * Revision history: Creado el 24 de enero de 2022
 */

#ifndef ADC7seg_H
#define	ADC7seg_H

#include <xc.h>                         // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <stdio.h>

//-------------------------Variables-----------------------------------------------
uint8_t  disp_selector = 0b001;         // Variable para seleccionar el display adecuado
int unidades = 0;                       // Variable unidades que guarda el valor de ADRESH
int decenas = 0;                        // Variable que se muestra en segundo display
int dig[3];                             // Array para descomponer números en dígitos
uint8_t Puerto_A = 0;                   // Variable que guarda valor de PORTA

//-------------------------Función para obtener valor de ADC-----------------------
void ADC(void);                         // Función para extraer valor de POT
int tabla(int a);                       // Función de traducción a display de 7 segmentos

#endif	/* ADC7seg_H */

