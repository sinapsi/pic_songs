/*
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
 *  - disable watchdog timer
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
#define C (3816) //do 262hz
#define Cd (3609) //do#
#define D (3400) //re
#define Dd (3203) //re#
#define E (3029) //mi
#define F (2864) //fa
#define Fd (2702) //fa#
#define G (2550) //sol
#define Gd (2408) //sol#
#define A (2271) //la
#define Ad (2144) //la#
#define B (2023) //si


// variabili globali
int PWM1=1,PWM2=1,PWM3=1;
int i=0, j=0;
int input = 0;
int PERIODO=1;
int LUNGHEZZA = 1;





// plafone
#pragma idata bigdata

#define x 125
int song[x]=  {B,   A,  B,  A,  B,  C/2,    D/2,    C/2,    B,  A,  E,  A,
               Fd,  E,  Fd, E, B*2, A*2,    B*2,
               D,   Cd, B*2,    A*2,    G*2,    A*2,    G*2,    Fd*2,    E*2,
    /*5*/      Fd*2,    G*2,    A*2,    B*2,    Cd, D,  E,  G,  E,  G,
               F,   E,  D,  C,  B*2,    C,  A*2,
    /*9*/      G,   F,  E,  D,  C,      C/2,    B,  A,  G,  D/2,  C/2,    B,  A,  G,
    /*11*/     B,   A,  D,  E,  Fd,     G,  Fd,  A*2,    G*2,    Fd*2,   G*2,
    /*12*/     A*2, A*2,    B*2,    D,  Cd,  B*2,    Cd,
    /*13*/     P,   B*2,    Cd,  D,  E,  Fd,  Gd,  A,  Gd,  Fd,  Cd,  A,
    /*14*/     Gd,  Fd,  F, Cd,   Cd,  Cd/2,    B,  A,  Gd,  Cd/2,    A/2, Gd/2,
    /*15*/     Fd,  B*2,    Cd,  D,  E,  Fd,  Gd,  A,  Gd,  Fd,  Cd,  A,
    /*16*/     Cd/2, D/2,    E/2,    D/2,    Cd/2,    B,  A,  B,  Cd/2,    D/2,    E/2,    Cd/2};
    //*17*/     B,   A,  Gd, Fd,  E,  B,  Fd,  E,  Dd*2 };
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
    /*16*/     1,   1,      1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    //*17*/     1,   1,   1,   1,   1,   1,   2, 2, 2};



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
        PERIODO = (song[i])/2; //LA alto
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
