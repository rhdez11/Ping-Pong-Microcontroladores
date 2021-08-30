/*
 * File:   parcial_PingPong.c
 * Author: Ricardo Hernandez
 *
 * Created on 6 de septiembre de 2019, 04:39 PM
 */
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = XT_XT     // Oscillator Selection bits (XT oscillator (XT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
 
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

char rutinaInicial[]={0b01000010,0b00011000,0b00100100,0b10000001,0b00100100,0b00011000};

char leds[]={0b10000000,0b00100000,0b00001000,0b00000010,
0b01000000,0b00010000,0b00000100,0b00000001};

char displayIzq[]={0b00111111,0b00001100,0b00101011,0b00101101,0b00011100,0b00110101,
0b00110111,0b00101100,0b00111111,0b00111101};

char displayIzqB[]={0b00000000,0b00000000,0b00000001,0b00000001,0b00000001,
0b00000001,0b00000001,0b00000000,0b00000001,0b00000001};

char displayDer[]={0b00001111,0b00000100,0b00011011,0b00011101,0b00010100,0b00011101,0b00011111,
0b00001100,0b00011111,0b00011101};

char displayDerB[]={0b11000000,0b10000000,0b10000000,0b10000000,0b11000000,0b01000000,
0b01000000,0b10000000,0b11000000,0b11000000};

int niveles[]={23,20,17,12,9};

int gameStart;//0 false, 1 true
int pointScored;//0 false, 1 true
int pointLost;
int gameWon;//0 false, 1 true
int bounced;

int player1;
int player2;
int nivel;

void RutinaInicial(){
    gameStart=0;
    //empieza la rutina inicial
    do{
        for(int i=0;i<7;i++){
            PORTB=rutinaInicial[i];
            __delay_ms(80);
            
            //para hasta que se presionen los dos push buttons
            if(PORTDbits.RD6==1&&PORTDbits.RD5==1){
                gameStart=1;
            }
            
        }    
    }while(gameStart==0);
    
    if(gameStart==1){
        //se muestran la cuenta regresiva de 3 a 0 segundos en los displays
        LATA=displayIzq[3];
        LATE=displayIzqB[3];
        LATD=displayDer[3];
        LATC=displayDerB[3];
        __delay_ms(1000);
        LATA=displayIzq[2];
        LATE=displayIzqB[2];
        LATD=displayDer[2];
        LATC=displayDerB[2];
        __delay_ms(1000);
        LATA=displayIzq[1];
        LATE=displayIzqB[1];
        LATD=displayDer[1];
        LATC=displayDerB[1];
        __delay_ms(1000);
        LATA=displayIzq[0];
        LATE=displayIzqB[0];
        LATD=displayDer[0];
        LATC=displayDerB[0];

        __delay_ms(1000);
    }
    
    gameWon=0;
}

void NivelDelay(int x){
    
    for(int i=0;i<niveles[x];i++){
        __delay_ms(5);
    }
}

int CalcularNivel(){
    if(player1>player2){
        return player1;
    }else{
        return player2;
    }
}

void main(void){
    
    TRISB=0b00000000;//leds
    TRISD=0b11000000;//push buttons y display derecha
    TRISC=0;//display der
    TRISE=0;//display izq
    TRISA=0;//display izq
    
    //CONFIGURAR EL MODO PWM en el pin RC2
    CCP1CON = 0b00111100;
    //ESTABLECER LA FRECUENCIA DE OPERACIÓN (UTILIZANDO LA FÓRMULA)
    //PR2 = 249;
    //ESTABLECER EL PREESCALER, EN ESTE CASO 1:16
    T2CONbits.T2CKPS0 = 0;
    T2CONbits.T2CKPS1 = 1;
    //ACTIVAR EL TIMER2
    T2CONbits.TMR2ON = 0; 
    //ESTABLECER EL CICLO DE TRABAJO,10 BITS EN TOTAL, EN ESTE CASO EL //NÚMERO 512 EN BINARIO: 10000000000
    CCPR1L = 0b10000000;
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;
    //CONFIGURAR PUERTO C COMO SALIDA (SÓLO SE UTILIZARÁ RC2)
    TRISC = 0;
    
    while(1){
        LATA=0b00000000;
        LATE=0b00000000;
        LATC=0b00000000;
        LATD=0b00000000;
        
        gameWon=0;
        player1=0;//lado de los verdes
        player2=0;//lado de los amarillos

        int num=7;  //variable auxiliar utilizada para regresar en el arreglo
                //en la secuencia de leds
    
        RutinaInicial();// se manda a llamar la funcion de la rutina inicial
    
        //empieza el juego
        while(gameWon==0){

            bounced=0;
            pointScored=0;
            pointLost=0;
            
            //secuencia de amarillos a verdes
            if(player2<5){
                while(bounced==0){
                    nivel=CalcularNivel();
                    for(int i=0;i<8;i++){
                        //if(pointScored==0){                
                            LATB=leds[i];                
                            if(i==7){
                                //__delay_ms(2000);
                                NivelDelay(nivel);
                                NivelDelay(nivel);
                            }else{
                                NivelDelay(nivel);
                            }
                            if(PORTDbits.RD5==1){                    
                                if(i==7){
                                    bounced=1;
                                    T2CONbits.TMR2ON = 1;
                                    //3135.96 Hz
                                    PR2 = 0b00010011 ;
                                    T2CON = 0b00000111 ;
                                    CCPR1L = 0b00001001 ;
                                    CCP1CON = 0b00111100 ;
                                    __delay_ms(100);
                                    T2CONbits.TMR2ON = 0;
                                }
                            }                
                        //}
                        
                    }
                    if(bounced==0){
                        player2++;
                        LATA=displayIzq[player2];
                        LATE=displayIzqB[player2];
                        LATB=0b00000000;
                        pointLost=1;
                        if(player2==5){
                            gameWon=1;
                            bounced=1;
                        }
                        __delay_ms(1000);
                        T2CONbits.TMR2ON = 1;
                        //2093    Hz
                        PR2 = 0b01110110 ;
                        T2CON = 0b00000101 ;
                        CCPR1L = 0b00111011 ;
                        CCP1CON = 0b00011100 ;
                        __delay_ms(1000);
                        T2CONbits.TMR2ON = 0;
                            
                    }
                }
                
                pointLost=0;
                pointScored=0;
            }
                
                
            if(gameWon==0){
                bounced=0;
              //secuencia de verdes a amarillos
                while(bounced==0){
                    nivel=CalcularNivel();
                    num=7;
                    for(int i=0;i<8;i++){
                        //if(pointScored==0){
                            LATB=leds[num];

                            if(num==0){
                                NivelDelay(nivel);
                                NivelDelay(nivel);
                            }else{
                                NivelDelay(nivel);
                            }

                            if(PORTDbits.RD6==1){
                                if(num==0){
                                    bounced=1;
                                    T2CONbits.TMR2ON = 1;
                                    //2349.32 Hz
                                    PR2 = 0b01101001 ;
                                    T2CON = 0b00000101 ;
                                    CCPR1L = 0b00110100 ;
                                    CCP1CON = 0b00111100 ;
                                    __delay_ms(100);
                                    T2CONbits.TMR2ON = 0;
                                }
                            }
                                num--;
                        //}
                    }
                    if(bounced==0){
                        player1++;
                        LATD=displayDer[player1];
                        LATC=displayDerB[player1];
                        LATB=0b00000000;
                        pointLost=1;
                        if(player1==5){
                            gameWon=1;
                            bounced=1;
                        }
                        
                        __delay_ms(1000);
                        T2CONbits.TMR2ON = 1;
                        //2093    Hz
                        PR2 = 0b01110110 ;
                        T2CON = 0b00000101 ;
                        CCPR1L = 0b00111011 ;
                        CCP1CON = 0b00011100 ;
                        __delay_ms(1000);
                        T2CONbits.TMR2ON = 0;
                    }
                }
            }
                
                pointLost=0;
                pointScored=0;
                bounced=0;
        }
        
        //(do, do , sol) 2093Hz, 190ms,silencio durante 190ms, 2093Hz, 230ms, 3135.96 Hz,
        //500ms, silencio.
            __delay_ms(500);
            T2CONbits.TMR2ON = 1;
            //2093    Hz
            PR2 = 0b01110110 ;
            T2CON = 0b00000101 ;
            CCPR1L = 0b00111011 ;
            CCP1CON = 0b00011100 ;
            __delay_ms(190);
            T2CONbits.TMR2ON = 0;
            __delay_ms(190);
            T2CONbits.TMR2ON = 1;
            //2093    Hz
            PR2 = 0b01110110 ;
            T2CON = 0b00000101 ;
            CCPR1L = 0b00111011 ;
            CCP1CON = 0b00011100 ;
            __delay_ms(190);
            //3135.96 Hz
            PR2 = 0b00010011 ;
            T2CON = 0b00000111 ;
            CCPR1L = 0b00001001 ;
            CCP1CON = 0b00111100 ;
            __delay_ms(230);
            T2CONbits.TMR2ON = 0;
            __delay_ms(500);
            
            if(player1==5){
                for(int i=0;i<16;i++){
                    LATD=0b00000000;
                    LATC=0b00000000;
                    __delay_ms(250);
                    LATD=displayDer[player1];
                    LATC=displayDerB[player1];    
                    __delay_ms(250);            
                }
            }else if(player2==5){
                for(int i=0;i<16;i++){
                    LATA=0b00000000;
                    LATE=0b00000000;
                    __delay_ms(250);
                    LATA=displayIzq[player2];
                    LATE=displayIzqB[player2];
                    __delay_ms(250);
                }       
            }        
    }
        
}   


