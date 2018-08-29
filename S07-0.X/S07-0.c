/*
 * File:   S07-0.c
 * Author: pc
 *
 * Created on 2018?8?28?, ??11:01
 */
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


#include <xc.h>
 //????
 unsigned char sin = 0;
 unsigned char getData = 0;

//???
void INT(){
    //??
    OSCCONbits.IRCF = 13;
    OSCCONbits.SCS = 3;
    //????
    APFCON1bits.TXSEL = 1; //0???RC6??1???RB6?    ??
    APFCON1bits.RXSEL = 1;  //0???RC7??1???RB7?   ??
    //????
//    APFCON1=0b00000000;
    
    TRISB=0b00000000;
    LATB=0b11011111;
    TRISB=0b00100000;
    LATB=0b11011111;
    //EUSART
    //??
    BAUDCON=0b01001001; //???
    TXSTAbits.BRGH=1;//??
    
    TXSTAbits.TXEN=1;
    TXSTAbits.SYNC=0; //????
    RCSTAbits.SPEN =1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    
    //??
  
    
    //????
    SPBRGH=0x03;
    SPBRGL=0x40;
//    PIE1bits.TXIE=1; //USART????
}
//??
void interrupt ISR(){
    //??
    if(PIR1bits.TXIF == 1){
        sin = sin + 1;
    }
    //??
    if(PIR1bits.RCIF == 1){
        getData = RCREG;
    }
    PIE1bits.TXIE=0; //USART????
//    PIR1bits.TXIF = 1;//?????????
    
}

//????
void sendMessage(){
    //?????TXREG????????
    TXREG = 77;
    
//    PIE1bits.TXIE=0; //USART????
}

//????
void receive(){
    ANSELB=0b00000000;
    TXSTAbits.SYNC=0; //????
    RCSTAbits.CREN = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.RCIE=1;//??????
    PIR1bits.RCIF = 1;//?????????
}

void main(void) {
    INT();
    do{
        sendMessage();
        receive();
    }while(1);
    return;
}

