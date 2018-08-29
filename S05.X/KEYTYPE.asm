#include "p16F1786.inc"
    
; CONFIG1
; __config 0xFFFC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON
    ; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR
    UDATA_SHR
 KEY_FINAL res 1h  ;??
 KEY_BEFORE res 1h ;?????????
 STATE res 1h   ;??
 STATE_BEFORE res 1h;?????????
 M_CNT res 1h   ;??????
 N_CNT res 1h   ;??????
 R_LONG res 1h  ;?????
 L1_LONG res 1h   ;?????
 L2_LONG res 1h    ;?????
 L3_LONG RES 1h    ;?????
 TYPE res 1h   ;????
 TYPE_BEFORE res 1h
 CURRENT_COUNT RES 1h   ;??????main????10???
 
 udata_ovr
COUNT0 RES 1H
COUNT1 RES 1H
COUNT2 RES 1H
COUNT3 RES 1H
COUNT4 RES 1H
COUNT5 RES 1H
COUNT6 RES 1H
COUNT7 RES 1H
COUNT8 RES 1H
COUNT9 RES 1H
;?? 
DISPLAY_ONE
DISPLAY_TWO
DISPLAY_THREE
DISPLAY_FOUR
 

RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    INT                   ; go to beginning of program

; TODO ADD INTERRUPTS HERE IF USED

MAIN_PROG CODE                      ; let linker place main program

 
    
TABLENUM:
    ADDWF PCL,F
    RETLW B'00000001'
    RETLW B'01001111'
    RETLW B'00010010'
    RETLW B'00000110'
    RETLW B'01001100'
    RETLW B'00100100'
    RETLW B'00100000'
    RETLW B'00001111'
    RETLW B'00000000'
    RETLW B'00000100'

INT
 movlw b'01101011'
 banksel OSCCON
 movwf OSCCON
 banksel TMR0
 movlw b'11000000'
 movwf TMR0 
 banksel OPTION_REG
 movlw b'00000111'
 movwf OPTION_REG
 banksel INTCON
 movlw b'10100000'
 movwf INTCON 
 
 MOVLW 0x07
 MOVWF KEY_FINAL
 MOVLW 0x07
 MOVWF KEY_BEFORE
 MOVLW 0x01
 MOVWF STATE
 MOVLW 0x00
 MOVWF STATE_BEFORE
 
 MOVLW 0x80
 MOVWF M_CNT
 MOVLW 0x00
 MOVWF N_CNT
 MOVLW 0xFF
 MOVWF R_LONG
 
 MOVLW 0x0A
 MOVWF L3_LONG
 MOVLW 0x14
 MOVWF L1_LONG
 MOVLW 0x8E
 MOVWF L2_LONG
 
 MOVLW 0x04
 MOVWF TYPE
 MOVLW 0x04
 MOVWF TYPE_BEFORE
 MOVLW 0x01
 MOVWF CURRENT_COUNT
 
 GOTO START
 
;KEY????
CHANGE_KEY
 ;?KEY????KEY_BEFORE
 MOVF KEY_FINAL,0
 MOVWF KEY_BEFORE
 
 
 ;??STATE_BEFORE??1
 MOVLW B'00000001'
 MOVWF STATE_BEFORE
 ;????????
 MOVLW 0x80
 MOVWF M_CNT
 MOVLW 0x00
 MOVWF N_CNT
 MOVLW 0xFF
 MOVWF R_LONG
 RETURN
 
; no change key
NO_CHANGE_KEY
 ;?STATE?STATE_BEFORE????????????????
 MOVF STATE,0
 XORWF STATE_BEFORE,0
 SUBLW B'00000000'
 BTFSS STATUS,0
 GOTO INEQUAL
 GOTO EQUAL
    INEQUAL
	MOVF STATE,0
	SUBLW B'00000000'
	BTFSS STATUS,0
	GOTO ZERO_TO_ONE  ;0 -> 1
	GOTO ONE_TO_ZERO  ;1 -> 0
	
    EQUAL 
	MOVF STATE,0
	SUBLW B'00000000'
	BTFSS STATUS,0
	GOTO ONE_TO_ONE    ;1=1,1 -> 1
	GOTO ZERO_TO_ZERO   ;0=0,0 -> 0
	
    ONE_TO_ZERO
	INCF N_CNT
	GOTO NO_CHANGE_KEL_END ;?1?0?????????
	
    ZERO_TO_ONE
	INCF M_CNT
	MOVLW B'00000000'
	MOVWF R_LONG  ;?????????
	GOTO NO_CHANGE_KEL_END ;?0?1?????????
	
    ZERO_TO_ZERO
	INCF N_CNT ;N_CNT++
	MOVF N_CNT,0 ;??N_CNT?????2
	SUBLW B'00000010'
	BTFSC STATUS,0
	GOTO CLEAR_M_CNT;M_CNT=0
	GOTO ZERO_CONTINUE
	
	CLEAR_M_CNT
	    MOVLW B'00000000'
	    MOVWF M_CNT
	    GOTO NO_CHANGE_KEL_END
	    
	ZERO_CONTINUE
	    MOVF R_LONG,0  ;??R<L3,
	    SUBWF L3_LONG,0
	    BTFSC STATUS,0
	    GOTO DOUBLE_PRESS;?????
	    GOTO ZERO_CONTINUE_CONTIUNE;??
	    
	DOUBLE_PRESS
	    MOVLW B'00000001'
	    MOVWF TYPE  ;01????
	    INCF L2_LONG,0  ;??N_CNT=L2_LONG+1
	    MOVWF N_CNT
	    GOTO NO_CHANGE_KEL_END
	    
	ZERO_CONTINUE_CONTIUNE
	    MOVF L2_LONG,0 ;??L2 = N ?
	    XORWF N_CNT,0
	    SUBLW B'0000000'
	    BTFSS STATUS,0
	    GOTO NO_CHANGE_KEL_END  ;??
	    GOTO LONG_PRESS  ;????? ;??
	    
	LONG_PRESS
	    MOVLW B'00000010'
	    MOVWF TYPE ;10????
	    GOTO NO_CHANGE_KEL_END
 
    ONE_TO_ONE
	INCF M_CNT ;M_CNT++
	INCF R_LONG ;R_LONG++
	MOVF M_CNT,0 ;??M_CNT?????2
	SUBLW B'00000010'
	BTFSC STATUS,0
	GOTO CLEAR_N_CNT;N_CNT=0
	GOTO ONE_CONTINUE
	
	CLEAR_N_CNT
	    MOVLW B'00000000' ;??N_CNT
	    MOVWF N_CNT
	    GOTO NO_CHANGE_KEL_END
	    
	ONE_CONTINUE
	    MOVF L2_LONG,0  ;??L2<N_CNT,
	    SUBWF N_CNT,0
	    BTFSC STATUS,0
	    GOTO ADD_M_CNT;??M=L1+1
	    GOTO ONE_CONTINUE_CONTIUNE;??
	    
	ADD_M_CNT
	    INCF L1_LONG,0 ;??M=L1+1
	    MOVWF M_CNT
	    GOTO ONE_CONTINUE_CONTIUNE;??
	  
	ONE_CONTINUE_CONTIUNE
	    MOVF L1_LONG,0 ;??L1 = M ?
	    XORWF M_CNT,0
	    SUBLW B'0000000'
	    BTFSS STATUS,0
	    GOTO NO_CHANGE_KEL_END  ;??
	    GOTO SHORT_PRESS  ;????? ;??
	    
	SHORT_PRESS
	    MOVLW B'0000000'
	    MOVWF TYPE
	    GOTO NO_CHANGE_KEL_END
 
 NO_CHANGE_KEL_END
 ;??STATE_BEFORE
 MOVF STATE,0
 MOVWF STATE_BEFORE
 
 RETURN
 

COUNT_CHANGE
 ;??TYPE????
 MOVF TYPE,0
 XORWF TYPE_BEFORE,0
 SUBLW B'00000000'
 BTFSS STATUS,0
 CALL CHANGE_TYPE;change key ??
 GOTO COUNT_CHANGE_END;NO change ??
 
    CHANGE_TYPE
	 ;??KEY???count
	 MOVF KEY_FINAL,0
	 SUBLW B'00000000'
	 BTFSC STATUS,0
	 CALL COUNT_ONE;NO change ??
;	 CALL CHANGE_KEY;change key ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000001'
	 BTFSC STATUS,0
	 CALL COUNT_TWO;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000010'
	 BTFSC STATUS,0
	 CALL COUNT_THREE;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000011'
	 BTFSC STATUS,0
	 CALL COUNT_FOUR;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000100'
	 BTFSC STATUS,0
	 CALL COUNT_FIVE;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000101'
	 BTFSC STATUS,0
	 CALL COUNT_SIX;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000110'
	 BTFSC STATUS,0
	 CALL COUNT_SEVEN;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00000111'
	 BTFSC STATUS,0
	 CALL COUNT_EIGHT;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00001000'
	 BTFSC STATUS,0
	 CALL COUNT_NINE;NO change ??
	 MOVF KEY_FINAL,0
	 SUBLW B'00001001'
	 BTFSC STATUS,0
	 CALL COUNT_TEN;NO change ??
	 MOVLW B'00000000' ;KEY=FF
	 MOVWF CURRENT_COUNT
	 GOTO COUNT_CHANGE_END  ;KEY???0-9
	 
	 COUNT_ONE
	    INCF COUNT0
	    MOVF COUNT0,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_TWO
	    INCF COUNT1
	    MOVF COUNT1,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_THREE
	    INCF COUNT2
	    MOVF COUNT2,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_FOUR
	    INCF COUNT3
	    MOVF COUNT3,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_FIVE
	    INCF COUNT4
	    MOVF COUNT4,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_SIX
	    INCF COUNT5
	    MOVF COUNT5,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_SEVEN
	    INCF COUNT6
	    MOVF COUNT6,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_EIGHT
	    INCF COUNT7
	    MOVF COUNT7,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_NINE
	    INCF COUNT8
	    MOVF COUNT8,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 COUNT_TEN
	    INCF COUNT9
	    MOVF COUNT9,0
	    MOVWF CURRENT_COUNT
	    GOTO COUNT_CHANGE_END
	 
 COUNT_CHANGE_END
 RETURN
 ;????
 DISPLAY
     MOVF KEY_FINAL,0
     SUBLW B'11111111'
     BTFSS STATUS,0
     CALL DISPALY_KEY;change  ??
     CALL LIGHTNING;NO change ??
     
     LIGHTNING
	MOVLW B'11111110'
	MOVWF DISPLAY_ONE
	MOVWF DISPLAY_TWO
	MOVWF DISPLAY_THREE
	MOVWF DISPLAY_FOUR
	GOTO DISPALY_END
     
     DISPALY_KEY
	;???
	MOVF KEY_FINAL
	CALL TABLENUM
	MOVWF DISPLAY_ONE
	;???
	MOVF TYPE
	CALL TABLENUM
	MOVWF DISPLAY_TWO
	;????
	MOVF CURRENT_COUNT
	ANDLW B'00000111'
	CALL TABLENUM
	MOVWF DISPLAY_FOUR
	
	LSRF CURRENT_COUNT,1
	LSRF CURRENT_COUNT,1
	LSRF CURRENT_COUNT,1
	MOVF CURRENT_COUNT
	ANDLW B'00000111'
	CALL TABLENUM
	MOVWF DISPLAY_THREE
	
	GOTO DISPALY_END
     
DISPALY_END
 RETURN
    
 
START
 loop
 ;??key????
 MOVF KEY_FINAL,0
 XORWF KEY_BEFORE,0
 SUBLW B'00000000'
 BTFSS STATUS,0
 CALL CHANGE_KEY;change key ??
 CALL NO_CHANGE_KEY;NO change ??
 
 ;??type????????????count
 CALL COUNT_CHANGE
 
 CALL DISPLAY;????
 
 goto loop
 
 

    END