#include "p16F1786.inc"

; CONFIG1
; __config 0xFFFC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON
;udata_shr
; snum0 res 00000001B
; snum1 res 10011111B
; snum2 res 00010010B
; snum3 res 00000110B
; snum4 res 01001100B
; snum5 res 00100100B
; snum6 res 00100000B
; snum7 res 00001111B
; snum8 res 00000000B
; snum9 res 00000100B
 org 0 
 RST	code 0x0
	pagesel main
	goto main
isr     code 0x4
	movlw b'01100100'
	XORwf 0x70
	
	banksel LATA
	movlw b'00000000'
	xorwf LATA,1
	
	BANKSEL INTCON
	BCF INTCON, INTF
	BANKSEL IOCBF
        MOVLW B'00000000'
	MOVWF IOCBF
	retFIE

 main	
    BANKSEL ANSELB
    CLRF  ANSELB
    BANKSEL INTCON
    BSF INTCON, PEIE
    BSF INTCON, GIE
    BSF INTCON, INTE
    BCF INTCON, INTF
    BSF INTCON,IOCIE
    BANKSEL TRISA
    CLRF TRISA
    BANKSEL TRISB
    movlw B'00000001'
    movwf TRISB
    BANKSEL IOCBP
    bSf IOCBP,0
    BSF IOCBP,5
    BANKSEL IOCBN
    bCf IOCBN,0
    BSF IOCBN,5
    BANKSEL IOCBF
    MOVLW B'00000000'
    MOVWF IOCBF
    
    BANKSEL OPTION_REG
    BSF OPTION_REG, INTEDG  ;pos edge

	movlw b'10000110'
	movwf 0x70
	movlw b'01001011'	
        banksel OSCCON
	movwf OSCCON
	movlw b'00000000'
	banksel TRISA
	movwf TRISA
	banksel TMR0
	movlw b'00000000'
	movwf TMR0
	banksel OPTION_REG
	movlw b'01000111'
	movwf OPTION_REG
	banksel LATA
	movlw b'00000000'
	movwf LATA
	banksel LATB
	movlw B'00010000'
	movwf LATB
	
;	bcf INTCON,2
;	bcf INTCON,3
;	bsf INTCON,4
;	bcf INTCON,5
;	bcf INTCON,6
;	bsf INTCON,7
	
loop
;	movlw b'11111111'
;	banksel LATB
;	xorwf LATB,1
	banksel LATB
	movlw b'00010000'
	movwf LATB
	movlw b'10000001'
	banksel LATA
	movwf LATA
	movlw b'00001000'
	movwf LATB
	movlw b'10000011'
	banksel LATA
	movwf LATA
	movlw b'00000100'
	movwf LATB
	movlw b'10000111'
	banksel LATA
	movwf LATA
	movlw b'00000010'
	movwf LATB
	movlw b'10001111'
	banksel LATA
	movwf LATA	
	banksel TMR0
	movf 0x70,0
	movwf TMR0
	banksel INTCON
	bcf INTCON,2
	banksel LATB
	movlw b'00010000'
	movwf LATB
	movlw b'10000001'
	banksel LATA
	movwf LATA
	movlw b'00001000'
	movwf LATB
	movlw b'10000011'
	banksel LATA
	movwf LATA
	movlw b'00000100'
	movwf LATB
	movlw b'10000111'
	banksel LATA
	movwf LATA
	movlw b'00000010'
	movwf LATB
	movlw b'10001111'
	banksel LATA
	movwf LATA
loop2
	
	banksel INTCON
	btfss INTCON,2
	goto loop2
	goto loop
	
end