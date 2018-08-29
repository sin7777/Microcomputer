/*
 * File:   S05-C.c
 * Author: pc
 *
 * Created on 2018?8?26?, ??11:14
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
//#include <p16F1786.inc>

//??????
unsigned char KEY_FINAL;
unsigned char key_before;
unsigned char STATE;
unsigned char state_before;
unsigned char m_cnt;
unsigned char n_cnt;
unsigned char r_count;
unsigned char short_standard; //??????L1
unsigned char long_standard;  //???? ?L2
unsigned char double_standard;  //??????L3
unsigned char type;
unsigned char current_count;
//??????
unsigned char count[10];
unsigned char LED[4];
//???
void INT(){
    KEY_FINAL = 255;
    STATE =1;
    type = 0; //?????
    short_standard = 10;
    long_standard = 50;
    double_standard = 20;
    current_count = 0;
    m_cnt = 128;
    n_cnt = 0;
    r_count = 255;
    key_before = 7;
    for(unsigned char i = 0; i<10; i++){
        count[i] = 0;
    }
};
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
};

//???
void keyChange(){
    key_before = KEY_FINAL;
    state_before = 1;
    //??????
    m_cnt = 128;
    n_cnt = 0;
    r_count = 255;
    return;
};
void noPress(){
    m_cnt++;
    r_count++;
    if(m_cnt == 2){
        n_cnt = 0;
        return;
    };
    if(type == 2 || type == 3){
        return; //???????
    };
    if(m_cnt == short_standard){
        type = 1; //??
        addCount();
    }
    return;
};
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
};

void showResult(){
    LED[0] = KEY_FINAL;
    LED[1] = type;
    LED[2] = current_count / 10; //?10??
    LED[3] = current_count % 10; //?10??
    return;
};


void main(void) {
    INT();
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
    state_before = STATE;  //?????
    showResult();
    return;
}
