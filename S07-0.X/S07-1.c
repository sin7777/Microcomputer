
// PIC16F1786 Configuration Bit Settings

// 'C' source line config statements

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <htc.h>
#include "pic16f1786.h"
#include <htc.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

#define uint8 unsigned char
#define uint16 unsigned int/*
 * File:   S_07c.c
 * Author: ???
 *
 * Created on 2018?8?28?, ??9:17
 */
unsigned char LED[4];
unsigned char SELECT_PORT=0b00001000;
unsigned char KEY_FINAL;
unsigned char STATE;
unsigned char key;
//unsigned char key_temp; 
unsigned char counter_n;
unsigned char counter0;
unsigned char counter_VOL=0;
unsigned char row;
unsigned char col ;
unsigned char portb_read;
unsigned char counter_col;
unsigned char KEY_FINAL16;
unsigned char i=0;
float VOL;
//key-type
unsigned char key_before;
unsigned char state_before;
unsigned char m_cnt;
unsigned char n_cnt;
unsigned char r_count;
unsigned char short_standard; //??????L1
unsigned char long_standard;  //???? ?L2
unsigned char double_standard;  //??????L3
unsigned char type;
unsigned char current_count;
unsigned char count[10];
unsigned char rcvMsg1=0;
unsigned char rcvMsg2=0;
unsigned char rcvMsg3=0;
void OSCinit(void){
    OSCCONbits.IRCF=0b1101;
};
void TMR0init(void){
    INTCONbits.TMR0IE = 1;
    OPTION_REGbits.TMR0CS = 0; // use Fosc/4 as timer source
    OPTION_REGbits.PSA = 0; // assign pre-scaler to Timer0
    // 1:8 pre-scaler rate
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;
    // Reset Timer0
    TMR0 = 0;

}
void FLASH(void){
    LATC=0b00000000;
    LATA=LED[i];
    SELECT_PORT=0b00001000>>i;
    ++i;
    if(i==4) i=0;
    LATC=SELECT_PORT;
    return;
}
void INTCONinit(void){
    INTCON=0b11101000;
}
void PORTinit(void){
    APFCON1=0b00000000;
    ANSELB=0b00000000;
    TRISB=0b00000000;
    LATB=0b11011111;
    TRISB=0b00100000;
    LATB=0b11011111;
    WPUB=0b11111111;
    TRISA=0b00000000;
    TRISC=0b10000000;
    LATC=0b00100000;
    WPUB=0b11111111;
    WPUC=0b00000000;
//    IOCCPbits.IOCCP7=1;
    IOCCNbits.IOCCN6=1;
    IOCCF=0b00000000;
}
void EUSARTinit(){
    TXSTAbits.TXEN=1;
    TXSTAbits.SYNC=0;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    TXSTAbits.BRGH = 1;
    BAUDCON=0b01001001;
    PIE1bits.TXIE=0;
    SPBRGH=0x03;
    SPBRGL=0x40;
    PIE1bits.RCIE=1; 
}
void RESET_KEY(){
    col=0b00001111;
    row=0b00001111;
    counter_col=0;
}
void CLEAN_KEY(){
    key=0xFF;
    STATE=1;
}
void CHECK_KEY(){
    STATE=0;
    switch(key){
        case 0x70:
            key=6;
            break;
        case 0x77:
            key=0;
            break;
        case 0x7B:
            key=1;
            break;
        case 0x7D:
            key=3;
            break;
        case 0xB8:
            key=7;
            break;
        case 0xBB:
            key=2;
            break;
        case 0xBD:
            key=4;
            break;
        case 0xDD:
            key=5;
            break;
        case 0xDC:
            key=8;
            break;
        case 0xEE:
            key=9;
            break;
        default:
            CLEAN_KEY();
    }
}
void GET_KEY(void){
    unsigned char mark[4]={0b11110000,0b11110001,0b11110011,0b11110111};
    unsigned char B[4]={0b11111111,0b11111110,0b11111101,0b11111011};
    TRISB=0b00000000;
    PORTB=B[counter_col];
    TRISB=0b00001111;
    portb_read=(unsigned char)(PORTB|mark[counter_col]);
    row=(unsigned char)(row&portb_read);
    if(portb_read!=0b11111111){
        col=(unsigned char)(col-(0b00000001<<counter_col));
    }
    ++counter_col;
    if(counter_col==4) {
        key=(unsigned char)((row<<4)|col);
        RESET_KEY();
        CHECK_KEY();
        if(KEY_FINAL!=key&& key!=0xff) KEY_FINAL=key;
    }
}
void GET_VOL(){
    if(ADCON0bits.GO==1){
        ADRESH=ADRESH&0b00001111;
        VOL=(1024.0/((int)ADRESH*256+ADRESL)*(4096*2-1)/2);
        ADCON0bits.GO=0;
    }
}
unsigned char SETNUM(int num){
    unsigned char code;
    switch((unsigned char)num){
        case 0:
            code=0b10000001;
            break;
        case 1:
            code=0b11001111;
            break;
        case 2:
            code=0b10010010;
            break;
        case 3:
            code=0b10000110;
            break;
        case 4:
            code=0b11001100;
            break;
        case 5:
            code=0b10100100;
            break;
        case 6:
            code=0b10100000;
            break;
        case 7:
            code=0b10001111;
            break;
        case 8:
            code=0b10000000;
            break;
        case 9:
            code=0b10000100;
            break;
        default:
            code=0b01111110;
    }
}
void DISPLAY_VOL(float VOL){
    LED[0]=SETNUM((int)(VOL/1000));
    LED[0]=LED[0]&0b01111111;
    VOL=(int)VOL%1000;
    LED[1]=SETNUM((int)(VOL/100));
    VOL=(int)VOL%100;
    LED[2]=SETNUM((int)(VOL/10));
    VOL=(int)VOL%10;
    LED[3]=SETNUM((int)(VOL));
}
//////////////////////////key-type-start/////////////////////////
void addCount(){
    switch(KEY_FINAL){
        case 0: 
            count[0]++;
            current_count = count[0];
            break;
        case 1: 
            count[1]++;
            current_count = count[1];
            break;
        case 2: 
            count[2]++;
            current_count = count[2];
            break;
        case 3: 
            count[3]++;
            current_count = count[3];
            break;
        case 4: 
            count[4]++;
            current_count = count[4];
            break;
        case 5: 
            count[5]++;
            current_count = count[5];
            break;
        case 6: 
            count[6]++;
            current_count = count[6];
            break;
        case 7: 
            count[7]++;
            current_count = count[7];
            break;
        case 8: 
            count[8]++;
            current_count = count[8];
            break;
        case 9: 
            count[9]++;
            current_count = count[9];
            break;
        default:current_count = 0;
    }
    return;
}
void keyChange(){
    key_before = KEY_FINAL;
    state_before = 1;
    //??????
    m_cnt = 128;
    n_cnt = 0;
    r_count = 255;
    return;
}
void noPress(){
    m_cnt++;
    r_count++;
    if(m_cnt == 2){
        n_cnt = 0;
        return;
    };
    if(r_count<double_standard){
        m_cnt = short_standard +1; //???????
    };
    if(m_cnt == short_standard){
        type = 1; //??
        addCount();
    }
    return;
}
void nowPress(){
    n_cnt++;
    if(n_cnt == 2){
        m_cnt = 0;
        return;
    };
    if(r_count < double_standard){
        type = 2; //??
        n_cnt = long_standard +1;//???????
    };
    if(n_cnt == long_standard){
        type = 3; //??
        addCount();
    }
    return;
}
void showResult(){
    LED[0] = SETNUM(KEY_FINAL);
    LED[1] = SETNUM(type);
    LED[2] = SETNUM(current_count / 10); //?10??
    LED[3] = SETNUM(current_count % 10); //?10??
    return;
}
void ACTION(void) {
    if(KEY_FINAL != key_before){
        keyChange();
    }
    if(state_before == 1 && STATE == 0){
        n_cnt = 1;
    }
    if(state_before == 0 && STATE == 1){
        m_cnt = 1;
        r_count = 0;
    }
    if(state_before == 1 && STATE == 1){
        noPress();
    }
    if(state_before == 0 && STATE == 0){
        nowPress();
    }
    state_before = STATE;  
    showResult();
    return;
}
/////////////////////////key-type-end///////////////////////
//------------------GET_MSG--------------------//
void delay_2us(){
#asm
#endasm
}
void RCV_MSG(){
    rcvMsg1=0;
    rcvMsg2=0;
    rcvMsg3=0;
    TRISCbits.TRISC5=0;
    PORTCbits.RC4=0;
    PORTCbits.RC5=1;delay_2us();
    PORTCbits.RC4=1;
    TRISCbits.TRISC5=1;//RS
    delay_2us();
    PORTCbits.RC4=0;
    int i2=0;
    while(i2<8){
        delay_2us();    
        PORTCbits.RC4=1;
        rcvMsg1=rcvMsg1|(LATCbits.LATC5<<i2);delay_2us();
        PORTCbits.RC4=0;
        ++i2;
    }
    TRISCbits.TRISC5=0;
    PORTCbits.RC5=0;delay_2us();    
    PORTCbits.RC4=1;delay_2us();    
    PORTCbits.RC4=0;
    i2=0;
    while(i2<8){
        delay_2us();    
        PORTCbits.RC4=1;
//        rcvMsg2=rcvMsg2|(PORTCbits.RC5<<(7-i2));
        rcvMsg2=rcvMsg2|(LATCbits.LATC5<<i2);
        PORTCbits.RC4=0;
        ++i2;
    }
    TRISCbits.TRISC5=0;
    PORTCbits.RC5=0;delay_2us();    
    PORTCbits.RC4=1;delay_2us();    
    PORTCbits.RC4=0;
    i2=0;
    while(i2<8){
        delay_2us();    
        PORTCbits.RC4=1;
//        rcvMsg3=rcvMsg3|(PORTCbits.RC5<<(7-i2));
        rcvMsg3=rcvMsg3|(LATCbits.LATC5<<i2);
        PORTCbits.RC4=0;
        ++i2;
    }
    TRISCbits.TRISC5=0;
    PORTCbits.RC5=1;delay_2us();    
    PORTCbits.RC4=1;delay_2us();    
    PORTCbits.RC4=0; 
    PORTCbits.RC5=0;delay_2us();    
    PORTCbits.RC4=1;
    PORTCbits.RC5=1;
    i2=0;
}
void GET_MSG(){
    unsigned char sendMsg1=0b00001001;
    TRISCbits.TRISC4=0;
    
    TRISCbits.TRISC5=0;
    PORTCbits.RC4=1;
    PORTCbits.RC5=1;
    int i1=0;
    PORTCbits.RC5=0;
    PORTCbits.RC4=0;
    while(i1<8){
        TRISCbits.TRISC5=0;
        PORTCbits.RC5=((sendMsg1>>i1)&0b00000001);delay_2us();
        TRISCbits.TRISC4=0;
        PORTCbits.RC4=1;delay_2us();
        TRISCbits.TRISC4=0;
        PORTCbits.RC4=0;
        ++i1;
    }
    PORTCbits.RC5=0;
    TRISCbits.TRISC5=1;delay_2us();
    PORTCbits.RC4=1;delay_2us();
    if(LATCbits.LATC5==0){
        TRISCbits.TRISC5=0;
        PORTCbits.RC4=0;
        i1=0;
        while(i1<8){
            PORTCbits.RC5=0;delay_2us();
            PORTCbits.RC4=1;delay_2us();
            PORTCbits.RC4=0;
            ++i1;
        }
        PORTCbits.RC5=0;
        TRISCbits.TRISC5=1;delay_2us();
        PORTCbits.RC4=1;delay_2us();
        if(LATCbits.LATC5==0){
            
            RCV_MSG();
            
        }
    }
}
void interrupt ISR(){
    if(INTCONbits.TMR0IF==1){
        TMR0=0b11100110;
        //ACTION();
        FLASH();
        GET_KEY();
        
        INTCONbits.TMR0IF=0;
        INTCON=0b11101000;
    }
    if(INTCONbits.IOCIF==1){
        //??eeprome
        eeprom_write(0x00, KEY_FINAL);
        eeprom_write(0x01, type);
        eeprom_write(0x02, current_count);
        IOCCF=0b00000000;
        IOCBF=0b00000000;
        INTCON=0b11101000;
    }
    if(PIR1bits.RCIF==1){
        LED[0]=RCREG;
        PIE1bits.RCIE=0;
    }
    
    return;
}

void main(void) {
    OSCinit();  
    FLASH();
    PORTinit;
    INTCONinit;
    TMR0init();
    LED[0]=0x00;
    LED[1]=0x00;
    LED[2]=0x00;
    LED[3]=0x00;
    
    do{
       PIE1bits.RCIE = 0;
        EUSARTinit();
       PIE1bits.TXIE = 1; 
       TXREG=0x11;
       PIE1bits.TXIE = 0;
#asm
        nop
        nop
        nop
        nop       
#endasm       
    }while(1);
    return;
}

