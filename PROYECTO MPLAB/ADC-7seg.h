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
uint8_t  disp_selector = 0b001;
int unidades = 0;
int decenas = 0;
int dig[3];
uint8_t Puerto_A = 0;

//-------------------------Función para obtener valor de ADC-----------------------
void ADC(void);
int tabla(int a);

#endif	/* ADC7seg_H */

