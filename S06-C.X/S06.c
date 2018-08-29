/*
 * File:   S06.c
 * Author: pc
 *
 * Created on 2018?8?26?, ??4:58
 */


#include <xc.h>
#include <htc.h>
//#include<eeprom_routines.h>

unsigned KEY_FINAL = 0x00;


void main(void) {
    unsigned char sin = 9;
    write(0x00,  KEY_FINAL);
    read(0x00);
    return;
}

void write(unsigned char addr,unsigned char value){
    EEADRL = addr;
    EEDATL = value;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    EECON1bits.WREN = 0;
    do{
        
    }while(EECON1bits.WR == 1);
    return 0;
}
char read(unsigned char addr){
    EEADRL = addr;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    unsigned char value = EEDATL;
    return value;
}


