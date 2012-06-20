/*
 *  * Copyright Michele Nicola Rappazzo <micheleee at gmail.com> 2012

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    wxpymaps is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    If not, see <http://www.gnu.org/licenses/>.
 *
 * PIC18F2431
 *
 *  Controllo tre canali PWM
 *
 *  PWM1 -> RB1
 *  PWM3 -> RB3
 *  PWM5 -> RB4
 *
 *
 */

#include <p18f2431.h>
#include <timers.h>  // libreria per timer
#include <delays.h>

// prototipi funzioni
void PWMset(void);
void PERset(void);
void delay_10ms(unsigned char i);
/*
 *  - set HS + PLL oscillator
 *  - disable wathCdog timer
 *  - disable low voltage programming
 */

#pragma config OSC  = HSPLL  // Oscillatore PLL 4x attivo
#pragma config WDTEN = OFF  // Whatch dog disattivato
#pragma config PWRTEN = ON  // Power UP timer abilitato
#pragma config LVP  = OFF  // Programmazione LVP disabilitata
#pragma config BOREN = OFF  // Brown out Reset disabilitato
#pragma config BORV  = 42  // Tensione per Brown out Reset 4.2 Volt
#pragma config PWMPIN = OFF

#define P 0 //pausa
#define C 1 //do 262hz
#define Cd 2 //do#
#define D 3 //re
#define Dd 4 //re#
#define E 5 //mi
#define F 6 //fa
#define Fd 7 //fa#
#define G 8 //sol
#define Gd 9 //sol#
#define A 10 //la
#define Ad 11 //la#
#define B 12 //si


#define lC 1+12 //do 262hz
#define lCd 2+12 //do#
#define lD 3+12 //re
#define lDd 4+12 //re#
#define lE 5+12 //mi
#define lF 6+12 //fa
#define lFd 7+12 //fa#
#define lG 8+12 //sol
#define lGd 9+12 //sol#
#define lA 10+12 //la
#define lAd 11+12 //la#
#define lB 12+12 //si

#define hC 24+1 //do 262hz
#define hCd 24+2 //do#
#define hD 24+3 //re
#define hDd 24+4 //re#
#define hE 24+5 //mi
#define hF 24+6 //fa
#define hFd 24+7 //fa#
#define hG 24+8 //sol
#define hGd 24+9 //sol#
#define hA 24+10 //la
#define hAd 24+11 //la#
#define hB 24+12 //si

int notes[37] = {0, 3816, 3609, 3400, 3203, 3029, 2864, 2702, 2550, 2408, 2271, 2144, 2023,
3816*2, 3609*2, 3400*2, 3203*2, 3029*2, 2864*2, 2702*2, 2550*2, 2408*2, 2271*2, 2144*2, 2023*2,
3816/2, 3609/2, 3400/2, 3203/2, 3029/2, 2864/2, 2702/2, 2550/2, 2408/2, 2271/2, 2144/2, 2023/2};


// variabili globali
int PWM1=1,PWM2=1,PWM3=1;
int i=0, j=0;
int input = 0;
int PERIODO;
int LUNGHEZZA = 1;





// plafone
#pragma idata bigdata

#define x 134
char song[x]=  {B,   A,  B,  A,  B,  hC,    hD,    hC,    B,  A,  E,  A,
               Fd,  E,  Fd, E, lB, lA,    lB,
               D,   Cd, lB,    lA,    lG,    lA,    lG,    lFd,    lE,
    /*5*/      lFd,    lG,    lA,    lB,    Cd, D,  E,  G,  E,  G,
               F,   E,  D,  C,  lB,    C,  lA,
    /*9*/      G,   F,  E,  D,  C,      hC,    B,  A,  G,  hD,  hC,    B,  A,  G,
    /*11*/     B,   A,  D,  E,  Fd,     G,  Fd,  lA,    lG,    lFd,   lG,
    /*12*/     lA, lA,    lB,    D,  Cd,  lB,    Cd,
    /*13*/     P,   lB,    Cd,  D,  E,  Fd,  Gd,  A,  Gd,  Fd,  Cd,  A,
    /*14*/     Gd,  Fd,  F, Cd,   Cd,  hCd,    B,  A,  Gd,  hCd,    hA, hGd,
    /*15*/     Fd,  lB,    Cd,  D,  E,  Fd,  Gd,  A,  Gd,  Fd,  Cd,  A,
    /*16*/     hCd, hD,    hE,    hD,    hCd,    B,  A,  B,  hCd,    hD,    hE,    hCd,
    /*17*/     B,   A,  Gd, Fd,  E,  B,  Fd,  E,  Dd*2 };
#pragma idata bigdata2

unsigned char length[x]={5,   1,  9,  1,  1,  1,      1,      1,      1,  1,  1,  1,
               5,   1,  5,  1,  10, 1,      1,
               2,   1,  1,      1,      1,      2,      1,      2,      1,
    /*5*/      3,       1,      1,      1,      1,  1,  1,  1,  1,  1,
               2,   1,  1,  6,  1,      1,  12,
    /*9*/      2,   1,  1,  1,  1,      2,      1,  1,  1,  3,      1,    2,  1,  6,
    /*11*/     1,   1,  1,  1,  1,      1,  2,  1,      1,      1,      1,
    /*12*/     6,   1,      1,      1,  1,  1,      1,
    /*13*/     1,   1,      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /*14*/     1,   1,  1,  1,   1,  1,       1,   1,   1,   1,       1,       1,
    /*15*/     1,   1,      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /*16*/     1,   1,      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /*17*/     1,   1,   1,   1,   1,   1,   2, 2, 2};



void main()
{
PORTA = 0x00;
PORTB = 0x00;
//PORTC = 0x00;

TRISA = 0xff;   // ingressi Analogici
TRISB = 0xff;   // uscite PWM
TRISC = 0x00;
//TRISCbits.TRISC4 = 1;
TRISCbits.TRISC3 = 0;
PORTC = 0x00;


INTCON = 0;     // disattiva tutti gli interrupt

/* inizio attivazione modulo PWM */
// RB1 (PWM1),RB3(PWM3),RB4(PWM5) PWM out
//PTCON0  = 0b00000000;  // 1:1 postscaler, 1:1 prescale, free running mode
PTCON0  = 0b00000100;  // 1:1 postscaler, 1:4 prescale, free running mode

//PTCON0  = 0b00001000;  // 1:1 postscaler, 1:16 prescale, free running mode
PTCON1  = 0b10000000;  // PWM time base ON, count up
PWMCON0 = 0b01110111;  // PWM1,PWM3,PWM5 abilitati in modo singolo
PWMCON1 = 0b00000000;  // 1:1 postscaler
// OVDCOND = 0b11111111;
// OVDCONS = 0b11111111;

DTCON = 0b01000101;   // dead time 500ns.
// DTCON = 0x00;   // dead time 0ns

FLTCONFIG = 0;

//PTPERH = 0x03;    // Carrier 9.8 kHz, 12 bit resolution (vedere tabella data sheet)
//PTPERL = 0xff;

PTPERH = 0x08;
PTPERH = 0xDF;

/* fine attivazione modulo PWM */

delay_10ms(20);
while(1){
    for (i=0;i<x;i++){
        //PORTC = ~PORTC;
        PERIODO = notes[song[i]]/2; //LA alto
        PWM1 = PERIODO/3;
        PWM2 = PERIODO/3;
        PWM3 = PERIODO/3;
        PERset();
        PWMset();
        LUNGHEZZA = length[i];
        
        for (j=0; j<LUNGHEZZA; j++){
            delay_10ms(4);
        }
        if (PERIODO !=1)
            PORTC = ~PORTC;
        PWM1 = 0;
        PWM2 = 0;
        PWM3 = 0;
        PERset();
        PWMset();
        delay_10ms(2);

        
    }



}
}
/****************** fine Main ****************/


void PWMset (void)
{
 if (PWM1>4095)PWM1=4095;
 if (PWM1<1) PWM1=1;
 PDC0L = PWM1;
 PDC0H = PWM1 >> 8;

 if (PWM2>4095)PWM2=4095;
 if (PWM2<1)    PWM2=1;
 PDC1L = PWM2;
 PDC1H = PWM2 >> 8;

 if (PWM3>4095)PWM3=4095;
 if (PWM3<1)   PWM3=11;
 PDC2L = PWM3;
 PDC2H = PWM3 >> 8;
}

void PERset (void){
 if (PERIODO>4095)PERIODO=4095;
 if (PERIODO<1) PERIODO=1;
 PTPERL = PERIODO;
 PTPERH = PERIODO >> 8;
}

volatile unsigned char dElAy;
void delay_10ms(unsigned char i){
  for (dElAy = 0; dElAy < i; dElAy++)
      Delay1KTCYx(100);
}
