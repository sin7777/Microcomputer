#include "p16F1786.inc"
;????
; CONFIG1
; __config 0xFFFC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON
 
 UDATA_SHR
 set1 res 1h
 set2 res 1h
 set3 res 1h
 set4 res 1h
 port res 1h
 set5 res 1h
 count res 1h
 delay res 1h
    ; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR
RST  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program

ISR CODE 0x0004  
    BANKSEL TMR0
    movlw b'11111110'
    movwf TMR0 ;
    call OUTPUT
    banksel INTCON
    movlw b'10100000'
    movwf INTCON
    RETURN
; TODO ADD INTERRUPTS HERE IF USED

    
MAIN_PROG CODE                      ; let linker place main program
DELAY_LED
 movlw B'00000000'
 INCFSZ delay,1
 BTFSC delay,4
 goto DELAY_LED
 CLRF delay
 return
RST_PORT
 movlw B'00001000'
 MOVWF port
 return
LED1
    BANKSEL LATC
    CLRF LATC    
    BANKSEL set1
    MOVF set1,0
    BANKSEL LATA
    MOVWF LATA     
    BANKSEL LATC
    MOVLW b'00001000'
    MOVWF LATC
    RETURN
LED2
    BANKSEL LATC
    CLRF LATC    
    BANKSEL set2
    MOVF set2,0
    BANKSEL LATA
    MOVWF LATA     
    BANKSEL LATC
    MOVLW b'00000100'
    MOVWF LATC
    RETURN
LED3
    BANKSEL LATC
    CLRF LATC    
    BANKSEL set3
    MOVF set3,0
    BANKSEL LATA
    MOVWF LATA     
    BANKSEL LATC
    MOVLW b'00000010'
    MOVWF LATC
    RETURN
LED4
    BANKSEL LATC
    CLRF LATC    
    BANKSEL set4
    MOVF set4,0
    BANKSEL LATA
    MOVWF LATA     
    BANKSEL LATC
    MOVLW b'00000001'
    MOVWF LATC
    MOVLW b'00010000'
    MOVWF port    
    RETURN
 OUTPUT
    BANKSEL LATC
    CLRF LATC     
    BTFSC port,3
    call LED1
    BTFSC port,2
    call LED2
    BTFSC port,1
    call LED3
    BTFSC port,0
    call LED4
    LSRF port,1
    RETURN
START
     movlw b'01101011';?????
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
    BANKSEL TRISC
    MOVWF TRISC
    
    MOVLW b'11111110'
    MOVWF set1   
    MOVLW b'11111101'
    MOVWF set2   
    MOVLW b'11111011'
    MOVWF set3   
    MOVLW b'11110111'
    MOVWF set4   
    MOVLW b'00001000'
    MOVWF port      
    
    loop    
    
    MOVLW b'11110001'
    MOVWF set1   
    MOVLW b'10000001'
    MOVWF set2   
    MOVLW b'11000001'
    MOVWF set3   
    MOVLW b'10110000'
    MOVWF set4  

    
    
    goto loop
    ; loop forever

    END
