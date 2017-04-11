//***********************************************************************
//man start
//&Switch Case template
//
//   -   below is the switch case structure develop for 'c'.
//   -   C - programmers can use full strings for reviewing case conditions.
//   -   users should not make changes to these macros unless additional cases
//       are needed.
//
//Carlos A. Perez
//man end
//***********************************************************************
//Super Loop Command Case
#define WRITE_STRING(stringName) sprintf(strTemp,"%s", stringName ); writeuart(strTemp);
#define SEND_RETURN_KEY_UART_MAIN uart8_putchar( drv_uart8_main, '\n');uart8_putchar( drv_uart8_main, '\r');
#define SEND_RETURN_KEY_UART1 uart8_putchar( drv_uart8_1, '\n'); uart8_putchar( drv_uart8_1, '\r');
#define SEND_RETURN_KEY_UART2 uart8_putchar( drv_uart8_2, '\n'); uart8_putchar( drv_uart8_2, '\r');
#define SEND_RETURN_KEY_UART3 uart8_putchar( drv_uart8_3, '\n'); uart8_putchar( drv_uart8_3, '\r');
#define SEND_RETURN_KEY_UART4 uart8_putchar( drv_uart8_4, '\n'); uart8_putchar( drv_uart8_4, '\r');
#define SEND_RETURN_KEY_ALL SEND_RETURN_KEY_UART_MAIN SEND_RETURN_KEY_UART1 SEND_RETURN_KEY_UART2 SEND_RETURN_KEY_UART3 SEND_RETURN_KEY_UART4


#define CASE_CLEAR_VAR memset( strCase, 0, sizeof( strCase ));
#define CASE_START(x) CASE_CLEAR_VAR strcpy(strCase, x); for(;;){
#define CASE_1(strCompare) if(compare_string(strCase, strCompare) == 0) {} else {goto case2;}
#define CASE_2(strCompare) {case2: if(compare_string(strCase, strCompare) == 0) {} else {goto case3;}}
#define CASE_3(strCompare) {case3: if(compare_string(strCase, strCompare) == 0) {} else {goto case4;}}
#define CASE_4(strCompare) {case4: if(compare_string(strCase, strCompare) == 0) {} else {goto case5;}}
#define CASE_5(strCompare) {case5: if(compare_string(strCase, strCompare) == 0) {} else {goto case6;}}
#define CASE_6(strCompare) {case6: if(compare_string(strCase, strCompare) == 0) {} else {goto case7;}}
#define CASE_7(strCompare) {case7: if(compare_string(strCase, strCompare) == 0) {} else {goto case8;}}
#define CASE_8(strCompare) {case8: if(compare_string(strCase, strCompare) == 0) {} else {goto case9;}}
#define CASE_9(strCompare) {case9: if(compare_string(strCase, strCompare) == 0) {} else {goto case10;}}
#define CASE_10(strCompare) {case10: if(compare_string(strCase, strCompare) == 0) {} else {goto case11;}}

#define CASE_11(strCompare) {case11: if(compare_string(strCase, strCompare) == 0) {} else {goto case12;}}
#define CASE_12(strCompare) {case12: if(compare_string(strCase, strCompare) == 0) {} else {goto case13;}}
#define CASE_13(strCompare) {case13: if(compare_string(strCase, strCompare) == 0) {} else {goto case14;}}
#define CASE_14(strCompare) {case14: if(compare_string(strCase, strCompare) == 0) {} else {goto case15;}}
#define CASE_15(strCompare) {case15: if(compare_string(strCase, strCompare) == 0) {} else {goto case16;}}
#define CASE_16(strCompare) {case16: if(compare_string(strCase, strCompare) == 0) {} else {goto case17;}}
#define CASE_17(strCompare) {case17: if(compare_string(strCase, strCompare) == 0) {} else {goto case18;}}
#define CASE_18(strCompare) {case18: if(compare_string(strCase, strCompare) == 0) {} else {goto case19;}}
#define CASE_19(strCompare) {case19: if(compare_string(strCase, strCompare) == 0) {} else {goto case20;}}
#define CASE_20(strCompare) {case20: if(compare_string(strCase, strCompare) == 0) {} else {goto case21;}}

#define CASE_21(strCompare) {case21: if(compare_string(strCase, strCompare) == 0) {} else {goto case22;}}
#define CASE_22(strCompare) {case22: if(compare_string(strCase, strCompare) == 0) {} else {goto case23;}}
#define CASE_23(strCompare) {case23: if(compare_string(strCase, strCompare) == 0) {} else {goto case24;}}
#define CASE_24(strCompare) {case24: if(compare_string(strCase, strCompare) == 0) {} else {goto case25;}}
#define CASE_25(strCompare) {case25: if(compare_string(strCase, strCompare) == 0) {} else {goto case26;}}
#define CASE_26(strCompare) {case26: if(compare_string(strCase, strCompare) == 0) {} else {goto case27;}}
#define CASE_27(strCompare) {case27: if(compare_string(strCase, strCompare) == 0) {} else {goto case28;}}
#define CASE_28(strCompare) {case28: if(compare_string(strCase, strCompare) == 0) {} else {goto case29;}}
#define CASE_29(strCompare) {case29: if(compare_string(strCase, strCompare) == 0) {} else {goto case30;}}
#define CASE_30(strCompare) {case30: if(compare_string(strCase, strCompare) == 0) {} else {goto case31;}}

#define CASE_31(strCompare) {case31: if(compare_string(strCase, strCompare) == 0) {} else {goto case32;}}
#define CASE_32(strCompare) {case32: if(compare_string(strCase, strCompare) == 0) {} else {goto case33;}}
#define CASE_33(strCompare) {case33: if(compare_string(strCase, strCompare) == 0) {} else {goto case34;}}
#define CASE_34(strCompare) {case34: if(compare_string(strCase, strCompare) == 0) {} else {goto case35;}}
#define CASE_35(strCompare) {case35: if(compare_string(strCase, strCompare) == 0) {} else {goto case36;}}
#define CASE_36(strCompare) {case36: if(compare_string(strCase, strCompare) == 0) {} else {goto case37;}}
#define CASE_37(strCompare) {case37: if(compare_string(strCase, strCompare) == 0) {} else {goto case38;}}
#define CASE_38(strCompare) {case38: if(compare_string(strCase, strCompare) == 0) {} else {goto case39;}}
#define CASE_39(strCompare) {case39: if(compare_string(strCase, strCompare) == 0) {} else {goto case40;}}


#define CASE_40(strCompare) {case40: if(compare_string(strCase, strCompare) == 0) {} else {goto case_end;}}
#define CASE_END {case_end: unknown_command(); break;}}
//***********************************************************************
//man start
//&Define chars
//
//   -   Use this section to declare any keyboard characters -> int values
//man end
//***********************************************************************
#define CR 13 /* this defines CR to be 13 */
#define LF 10 /* this defines LF to be 10 */
#define BS 127 /*back space*/
#define EOL 124 /*end of transmission*/

//***********************************************************************
//man start
//&Define user communication protocols
//
//   -   Use this section to declare common communication protocols
//   -   ideally you want to use this for displaying information to the end user
//   -   Please keep in mind that debugging at a hardware level it will require
//       UART SwPlatform knowledge.
//
//man end
//***********************************************************************
// Serial communication parameters, change to check what's happening
#define BAUDRATE    19200
#define PARITY      UART8_NO_PARITY
#define DATABITS    8
#define STOPBITS    1

//***********************************************************************
//man start
//&Error Commands
//
//   -   Use this defines to support multi-enviroment via bitwise.
//
//man end
//***********************************************************************
#define CHECK_ERRORS(x) if(x == 1) return 1;


//***********************************************************************
//man start
//&UARTS Commands
//
//   -   Use this defines to support multi-uart- interfaces
//       enviroment via bitwise.
//
//man end
//***********************************************************************

#define MAIN_UART 1       //instance 0
#define UART1 2           //instance 1
#define UART2 4           //instance 2
#define UART3 8           //instance 3
#define UART4 16          //instance 4
#define  NTHREADS 5       //Listeners threads
#define  RUNTIME_SCS                                 100
#define  FLASH_INTERVAL_MSCS                         100

#define SWITCH_TO_UART0 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(0);uart_active = is_uart_in_use(0);
#define SWITCH_TO_UART1 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(1);uart_active = is_uart_in_use(1);
#define SWITCH_TO_UART2 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(2);uart_active = is_uart_in_use(2);
#define SWITCH_TO_UART3 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(3);uart_active = is_uart_in_use(3);
#define SWITCH_TO_UART4 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(4);uart_active = is_uart_in_use(4);
#define SWITCH_TO_UART5 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(5);uart_active = is_uart_in_use(5);
#define SWITCH_TO_UART6 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(6);uart_active = is_uart_in_use(6);
#define SWITCH_TO_UART7 set_uart_unused(0);set_uart_unused(1);set_uart_unused(2);set_uart_unused(3);set_uart_unused(4);set_uart_unused(5);set_uart_unused(6);set_uart_unused(7);flip_uart_use_state(7);uart_active = is_uart_in_use(7);


#define CHECK_IF_UART_MAIN_IS_ACTIVE uart_active = is_uart_in_use(0);if( uart_active == MAIN_UART){SEND_RETURN_KEY_UART_MAIN WRITE_STRING("JBL>"); sendCommand(main_menu());continue;}
#define CHECK_IF_UART1_IS_ACTIVE uart_active = is_uart_in_use(1);if( uart_active == UART1){SEND_RETURN_KEY_UART_MAIN WRITE_STRING("JBL-UART1>"); sendCommand(uart1_menu());continue;}
#define CHECK_IF_UART2_IS_ACTIVE uart_active = is_uart_in_use(2);if( uart_active == UART2){SEND_RETURN_KEY_UART_MAIN WRITE_STRING("JBL-UART2>"); sendCommand(uart2_menu());continue;}
#define CHECK_IF_UART3_IS_ACTIVE uart_active = is_uart_in_use(3);if( uart_active == UART3){SEND_RETURN_KEY_UART_MAIN WRITE_STRING("JBL-UART3>"); sendCommand(uart3_menu());continue;}
#define CHECK_IF_UART4_IS_ACTIVE uart_active = is_uart_in_use(4);if( uart_active == UART4){SEND_RETURN_KEY_UART_MAIN WRITE_STRING("JBL-UART4>"); sendCommand(uart4_menu());continue;}











