#include "p16F1786.inc"

; CONFIG1
; __config 0xFFFC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON
 
 
    ; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR
    
    udata_shr
OFFSETA res 1h
OFFSETB res 1h
 
RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program

; TODO ADD INTERRUPTS HERE IF USED

MAIN_PROG CODE                      ; let linker place main program
TABLEA:
    ADDWF PCL,F
    RETLW B'00000001'
    RETLW B'01001111'
    RETLW B'00010010'
    RETLW B'00000110'
TABLEB:
    ADDWF PCL,F
    RETLW B'00000010'
    RETLW B'00000100'
    RETLW B'00001000'
    RETLW B'00010000'
    
isr  code 0x4
  
    banksel TMR0
    movlw b'11000000'
    movwf TMR0 ;???
    
    
    INCF OFFSETA,0
    CALL TABLEA
    BANKSEL LATA
    MOVWF LATA
    
    INCF OFFSETB,0
    CALL TABLEB
    BANKSEL LATB
    MOVWF LATB
    
    banksel INTCON
    bcf INTCON,2
    retFIE
    
    
    
START
     movlw b'01001011';?????
     banksel OSCCON
     movwf OSCCON
     banksel TMR0
     movlw b'11000000'
     movwf TMR0 ;???
     banksel OPTION_REG
     movlw b'00000111'
     movwf OPTION_REG;???
     banksel INTCON
     movlw b'10100000'
     movwf INTCON ;????
     
    MOVLW b'00000000'
    BANKSEL TRISA
    MOVWF TRISA
    MOVLW b'00000000'
    BANKSEL TRISB
    MOVWF TRISB
    MOVLW b'00010000'
    BANKSEL LATB
    MOVWF LATB
    MOVLW b'00000001'
     BANKSEL LATA
     MOVWF LATA
     
     CLRF OFFSETB
     CLRF OFFSETA
loop
     
   
    goto loop

    END


