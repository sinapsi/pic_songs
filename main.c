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

#define P &notes[0] //pausa
#define C &notes[1] //do 262hz
#define Cd &notes[2] //do#
#define D &notes[3] //re
#define Dd &notes[4] //re#
#define E &notes[5] //mi
#define F &notes[6] //fa
#define Fd &notes[7] //fa#
#define G &notes[8] //sol
#define Gd &notes[9] //sol#
#define A &notes[10] //la
#define Ad &notes[11] //la#
#define B &notes[12] //si


#define lC &lnotes[1] //do 262hz
#define lCd &lnotes[2] //do#
#define lD &lnotes[3] //re
#define lDd &lnotes[4] //re#
#define lE &lnotes[5] //mi
#define lF &lnotes[6] //fa
#define lFd &lnotes[7] //fa#
#define lG &lnotes[8] //sol
#define lGd &lnotes[9] //sol#
#define lA &lnotes[10] //la
#define lAd &lnotes[11] //la#
#define lB &lnotes[12] //si

#define hC &hnotes[1] //do 262hz
#define hCd &hnotes[2] //do#
#define hD &hnotes[3] //re
#define hDd &hnotes[4] //re#
#define hE &hnotes[5] //mi
#define hF &hnotes[6] //fa
#define hFd &hnotes[7] //fa#
#define hG &hnotes[8] //sol
#define hGd &hnotes[9] //sol#
#define hA &hnotes[10] //la
#define hAd &hnotes[11] //la#
#define hB &hnotes[12] //si

int notes[13] = {0, 3816, 3609, 3400, 3203, 3029, 2864, 2702, 2550, 2408, 2271, 2144, 2023};
int lnotes[13] = {0, 3816*2, 3609*2, 3400*2, 3203*2, 3029*2, 2864*2, 2702*2, 2550*2, 2408*2, 2271*2, 2144*2, 2023*2};
int hnotes[13] = {0, 3816/2, 3609/2, 3400/2, 3203/2, 3029/2, 2864/2, 2702/2, 2550/2, 2408/2, 2271/2, 2144/2, 2023/2};


// variabili globali
int PWM1=1,PWM2=1,PWM3=1;
int i=0, j=0;
int input = 0;
int PERIODO;
int LUNGHEZZA = 1;





// plafone
#pragma idata bigdata

#define x 125
int * song[x]=  {B,   A,  B,  A,  B,  hC,    hD,    hC,    B,  A,  E,  A,
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
    /*16*/     hCd, hD,    hE,    hD,    hCd,    B,  A,  B,  hCd,    hD,    hE,    hCd};
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
        PERIODO = *(song[i])/2; //LA alto
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
