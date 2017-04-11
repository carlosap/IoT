//******************************************************************************************
//man start
//&Includes
//
//   -   Add all Includes require for test / make sure uarts and i/o
//       functions
//
//Carlos A. Perez
//man end
//******************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <timing.h>
#include <devices.h>
#include <string.h>
#include "swplatform.h"
#include "Defines.c"

//*****************************Methods/Function Types*****************************************
void writeuart(char *karakter);
void clearScreen(void);
void uart_status(void);
void sendCommand(char *karakter);
void sendStringCommand(void);
void Trim_Command(void);
void show_ver(void);
void exit_prog(void);
void unknown_command(void);
void get_io_state(uint8_t getVal);
void verify_io_state(uint8_t getVal, uint8_t expectedVal);
void verify_IO_Value(uint8_t getVal, uint8_t expectedVal);
void verify_IO_ValueInverted(uint8_t getVal, uint8_t expectedVal);
void flip_uart_use_state(int uart_instance);

void start_listen_thread(void);
void uart1_listener_process(void *arg);
void uart2_listener_process(void *arg);
void uart3_listener_process(void *arg);
void uart4_listener_process(void *arg);


int set_IO_port(void);
int get_IO_port(void);
int check_IO_port(void);
int compare_string(char*, char*);
int get_command(void);
int set_uart_unused(int uart_instance);
int is_uart_in_use(int uart_instance);



//*****************************interfaces - CLIs********************************************
char *main_menu(void);     //Main CLI interface - Controls ALL IOs for the JBL UUT
char *uart1_menu(void);    //Echos user's input to the UART1 TX
char *uart2_menu(void);    //Echos user's input to the UART2 TX
char *uart3_menu(void);    //Echos user's input to the UART3 TX
char *uart4_menu(void);    //Echos user's input to the UART4 TX

//*****************************Variables and Pointers - CLIs********************************
char temp[128];
char strTemp[128];
char buf[128];
char strCase[30];
char *words[80];        //holds the words starting at index 0.
int wordCount;          // Number of commands
char strCmd[100];       //This variable holds the complete command
char strCmd1[50];       //1st Word of the command
char strCmd2[50];       //2nd Word of the command
char strCmd3[50];       //3rd Word of the command
char strCmd4[50];       //4th Word of the command
char strCmd5[50];       //5th Word of the command

//Save these Variables for Buffering Uarts Communications during Threads
char strCmd_uart1[200];
char strCmd_uart2[200];
char strCmd_uart3[200];
char strCmd_uart4[200];
char uart_in_use = 0;
int uart_active;

char strRemarks[200];
char strRemarks1[200];

//****************************CLI Global Variables******************************************
int cmdChar1, cmdSize1 = -1; char cmdLastChar1;
int cmdChar2, cmdSize2 = -1; char cmdLastChar2;
int cmdChar3, cmdSize3 = -1; char cmdLastChar3;
int cmdChar4, cmdSize4 = -1; char cmdLastChar4;

//****************************NTHREADS Variables******************************************
void *     retval[NTHREADS];
pthread_t  ids[NTHREADS];

//******************************************************************************************
//man start
//&main()
//
//   -   Program Starts HERE. Make sure to flip the correct state for the ACTIVE UART
//       instance. the CLI supports 5 UARTS and they are ACTIVE by setting the
//       flip_uart_use_state(0) function.
//
//Carlos A. Perez
//man end
//******************************************************************************************
int main (void)
{
     swplatform_init_stacks();
     SWITCH_TO_UART0
     show_ver();
     start_listen_thread();
     do
     {
         CHECK_IF_UART_MAIN_IS_ACTIVE   //Function Verifies State and Provides Interface Object- RS232 PC

         CHECK_IF_UART1_IS_ACTIVE       //Function Verifies State and Provides Interface Object- Uart1

         CHECK_IF_UART2_IS_ACTIVE      //Function Verifies State and Provides Interface Object- Uart2

         CHECK_IF_UART3_IS_ACTIVE      //Function Verifies State and Provides Interface Object- Uart3

         CHECK_IF_UART4_IS_ACTIVE      //Function Verifies State and Provides Interface Object- Uart4

     }while(strCmd != "quit");
     return 0;
}

//******************************************************************************************
//man start
//&char *main_menu()
//
//   -   This routine depended from the UART. you this technically
//       an "lrp" uart driven driver. if you desire to run another uart
//       copy and paste the same concept that you can re-use the
//       sendCommand(uart_menu) function.
//
//Carlos A. Perez
//man end
//******************************************************************************************
char *main_menu()
{
     int c;
     char cc;
     int index = -1;
     memset( strCmd, 0, sizeof( strCmd ));
     for (;;)
     {
            if ( c = uart8_getchar( drv_uart8_main ), c == -1 )continue;
            if (( c == 0x1b )||( c == 0x5b )||( c == 0x42 )||( c == 0x43 )||( c == 0x44 ))        //removing 41 - 65 -Shift A
            {
                   SEND_RETURN_KEY_UART_MAIN
                   WRITE_STRING("JBL>");
                   continue;
            }
            uart8_putchar( drv_uart8_main, c ); //Send to rs232 port
            if(c == BS)
            {
                if(strlen(strCmd)==0)
                {
                   WRITE_STRING(">");
                   SEND_RETURN_KEY_UART_MAIN
                   WRITE_STRING("JBL>");
                   continue;
                }
                index--;
                strCmd[index + 1] = '\0';
                continue;
            }
            else if(c == CR)
            {
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING("JBL>");
                break;
            }
            else
            {
                cc = c;
                index++;
                strCmd[index] = cc;
            }
     }
     Trim_Command();
     return strCmd;

}
//******************************************************************************************
//man start
//&char *uart1_menu()
//
//   -   This routine is a dedicated Uart instance 1. Note the the strCmd
//       it has it own dedicated vars.
//   -   see your defines.c for additonal details
//
//Carlos A. Perez
//man end
//******************************************************************************************
char *uart1_menu()
{
     int c;
     char cc;
     int index = -1;
     memset( strCmd, 0, sizeof( strCmd ));
     for (;;)
     {
       if ( c = uart8_getchar( drv_uart8_main ), c == -1 )continue;
       if (( c == 0x1b )||( c == 0x5b )||( c == 0x42 )||( c == 0x43 )||( c == 0x44 ))        //removing 41 - 65 -Shift A
       {
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART1>");
              continue;
       }
       uart8_putchar( drv_uart8_main, c ); //Send to rs232 port
       uart8_putchar( drv_uart8_1, c ); //Also send to uart1
       if(c == BS)
       {
           if(strlen(strCmd)==0)
           {
              WRITE_STRING(">");
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART1>");
              continue;
           }
           index--;
           strCmd[index + 1] = '\0';
           continue;
       }
       else if(c == CR)
       {
           SEND_RETURN_KEY_UART_MAIN
           WRITE_STRING("JBL-UART1>");
           break;
       }
       else
       {
           cc = c;
           index++;
           strCmd[index] = cc;
       }

     }
     Trim_Command();
     return strCmd;
}

//******************************************************************************************
//man start
//&char *uart2_menu()
//
//   -   This routine is a dedicated Uart instance 2. Note the the strCmd
//       it has it own dedicated vars.
//   -   see your defines.c for additonal details
//
//Carlos A. Perez
//man end
//******************************************************************************************
char *uart2_menu()
{
     int c;
     char cc;
     int index = -1;
     memset( strCmd, 0, sizeof( strCmd ));
     for (;;)
     {
       if ( c = uart8_getchar( drv_uart8_main ), c == -1 )continue;
       if (( c == 0x1b )||( c == 0x5b )||( c == 0x42 )||( c == 0x43 )||( c == 0x44 ))        //removing 41 - 65 -Shift A
       {
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART2>");
              continue;
       }
       uart8_putchar( drv_uart8_main, c ); //Send to rs232 port
       uart8_putchar( drv_uart8_2, c ); //Also send to uart2
       if(c == BS)
       {
           if(strlen(strCmd)==0)
           {
              WRITE_STRING(">");
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART2>");
              continue;
           }
           index--;
           strCmd[index + 1] = '\0';
           continue;
       }
       else if(c == CR)
       {
           SEND_RETURN_KEY_UART_MAIN
           WRITE_STRING("JBL-UART2>");
           break;
       }
       else
       {
           cc = c;
           index++;
           strCmd[index] = cc;
       }

     }
     Trim_Command();
     return strCmd;
}

//******************************************************************************************
//man start
//&char *uart3_menu()
//
//   -   This routine is a dedicated Uart instance 3. Note the the strCmd
//       it has it own dedicated vars.
//   -   see your defines.c for additonal details
//
//Carlos A. Perez
//man end
//******************************************************************************************
char *uart3_menu()
{
     int c;
     char cc;
     int index = -1;
     memset( strCmd, 0, sizeof( strCmd ));
     for (;;)
     {
       if ( c = uart8_getchar( drv_uart8_main ), c == -1 )continue;
       if (( c == 0x1b )||( c == 0x5b )||( c == 0x42 )||( c == 0x43 )||( c == 0x44 ))        //removing 41 - 65 -Shift A
       {
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART3>");
              continue;
       }

       uart8_putchar( drv_uart8_main, c ); //Send to rs232 port
       uart8_putchar( drv_uart8_3, c ); //Also send to uart3

       if(c == BS)
       {
           if(strlen(strCmd)==0)
           {
              WRITE_STRING(">");
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART3>");
              continue;
           }
           index--;
           strCmd[index + 1] = '\0';
           continue;
       }
       else if(c == CR)
       {
           SEND_RETURN_KEY_UART_MAIN
           WRITE_STRING("JBL-UART3>");
           break;
       }
       else
       {
           cc = c;
           index++;
           strCmd[index] = cc;
       }

     }
     Trim_Command();
     return strCmd;
}
//******************************************************************************************
//man start
//&char *uart4_menu()
//
//   -   This routine is a dedicated Uart instance 4. Note the the strCmd
//       it has it own dedicated vars.
//   -   see your defines.c for additonal details
//
//Carlos A. Perez
//man end
//******************************************************************************************
char *uart4_menu()
{
     int c;
     char cc;
     int index = -1;
     memset( strCmd, 0, sizeof( strCmd ));
     for (;;)
     {
       if ( c = uart8_getchar( drv_uart8_main ), c == -1 )continue;
       if (( c == 0x1b )||( c == 0x5b )||( c == 0x42 )||( c == 0x43 )||( c == 0x44 ))        //removing 41 - 65 -Shift A
       {
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART4>");
              continue;
       }
       uart8_putchar( drv_uart8_main, c );
       uart8_putchar( drv_uart8_4, c );
       if(c == BS)
       {
           if(strlen(strCmd)==0)
           {
              WRITE_STRING(">");
              SEND_RETURN_KEY_UART_MAIN
              WRITE_STRING("JBL-UART4>");
              continue;
           }
           index--;
           strCmd[index + 1] = '\0';
           continue;
       }
       else if(c == CR)
       {
           SEND_RETURN_KEY_UART_MAIN
           WRITE_STRING("JBL-UART4>");
           break;
       }
       else
       {
           cc = c;
           index++;
           strCmd[index] = cc;
       }

     }
     Trim_Command();
     return strCmd;
}

//******************************************************************************************
//man start
//&Trim_Command()
//
//   -   This routine trims my command string. command string should
//       nulled at the ends only.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void Trim_Command()
{
  //do standard commands
  for( unsigned int i = 0; i < strlen( strCmd ); i++ )
    if(( strCmd[i] == '\r' ) || ( strCmd[i] == '\n' ))
      strcpy( strCmd + i, strCmd + i + 1 );

   //do uart1 commands
  for( unsigned int i = 0; i < strlen( strCmd_uart1 ); i++ )
    if(( strCmd_uart1[i] == '\r' ) || ( strCmd_uart1[i] == '\n' ))
      strcpy( strCmd_uart1 + i, strCmd_uart1 + i + 1 );

  //do uart2 commands
  for( unsigned int i = 0; i < strlen( strCmd_uart2 ); i++ )
    if(( strCmd_uart2[i] == '\r' ) || ( strCmd_uart2[i] == '\n' ))
      strcpy( strCmd_uart2 + i, strCmd_uart2 + i + 1 );

  //do uart3 commands
  for( unsigned int i = 0; i < strlen( strCmd_uart3 ); i++ )
    if(( strCmd_uart3[i] == '\r' ) || ( strCmd_uart3[i] == '\n' ))
      strcpy( strCmd_uart3 + i, strCmd_uart3 + i + 1 );

  //do uart4 commands
    for( unsigned int i = 0; i < strlen( strCmd_uart4 ); i++ )
    if(( strCmd_uart4[i] == '\r' ) || ( strCmd_uart4[i] == '\n' ))
      strcpy( strCmd_uart4 + i, strCmd_uart4 + i + 1 );


}


//******************************************************************************************
//man start
//&writeuart(char *karakter)
//
//   -   This routine is a wrapper to avoid typing the size of the string
//       this routine supports pointer, and "quoted strings" but does not
//       support
//
//Carlos A. Perez
//man end
//******************************************************************************************
void writeuart(char *karakter)
{
    while( *karakter != 0)
    {
      //intf("Trying to write [%c]", *karakter);
      uart8_putchar(drv_uart8_main, *karakter);
      karakter++;
      delay_ms(1);
    }

}
//******************************************************************************************
//man start
//&clearScreen()
//
//   -   This routine clears the user's interface by adding \n\r loops
//
//Carlos A. Perez
//man end
//******************************************************************************************
void clearScreen()
{
     int i;
     for(i=0;i<200;i++)
     {
          SEND_RETURN_KEY_UART_MAIN
     }
}

//******************************************************************************************
//man start
//&sendCommand
//
//   -   This routine supports two type of commands: get_command() and
//       sendStringCommands().
//   -   get_command()- examines each "word" the user typed and tries to
//       structure a syntax algorimt.
//   -   sendStringCommand() - is an alternative to
//
//Carlos A. Perez
//man end
//******************************************************************************************
void sendCommand(char *karakter)
{

        int validCommand = get_command();                //
        if(validCommand <= 0)sendStringCommand();

}

//******************************************************************************************
//man start
//&sendStringCommand(char *karakter)
//
//   -   This routine is the key flow of the programm. the CASE_# are
//       technically MACROS (see the define.c)
//   -   the key feature is to be able to capture a complete string format
//       while making decisiions. the concepts is to control IOs via strings commnads.
//   -   'c' code does not support string case, and because "case" derives from old goto BASIC
//       similar approach was taken.
//   -   please make sure your cases are sequencially ordered: they are goto jumps.
//   -   if you are not using all of them just make sure you use "";
//
//Carlos A. Perez
//man end
//******************************************************************************************
void sendStringCommand()
{

                CASE_START(strCmd)

                        CASE_1("\0")  //just a return
                        break;

                        CASE_2("show version")
                        show_ver();
                        break;

                        CASE_3("clear")
                        clearScreen();
                        break;

                        CASE_4("")
                        break;

                       CASE_5("")
                       break;

                       CASE_6("exit")
                       exit_prog();
                       break;

                       CASE_7("start uart process")
                       start_listen_thread();
                       break;

                       CASE_8("")
                       break;

                       CASE_9("")
                       break;

                       CASE_10("")
                       break;

                       CASE_11("")

                       break;

                       CASE_12("")

                       break;

                       CASE_13("")
                       break;

                       CASE_14("")
                       break;

                       CASE_15("")
                       break;

                       CASE_16("jbl")
                       SWITCH_TO_UART0
                       break;

                       CASE_17("switch uart1")
                       SWITCH_TO_UART1
                       break;

                       CASE_18("switch uart2")
                       SWITCH_TO_UART2
                       break;

                       CASE_19("switch uart3")
                       SWITCH_TO_UART3
                       break;

                       CASE_20("switch uart4")
                       SWITCH_TO_UART4
                       break;

                       CASE_21("")
                       CASE_22("")
                       CASE_23("")
                       CASE_24("")
                       CASE_25("")
                       CASE_26("")
                       CASE_27("")
                       CASE_28("")
                       CASE_29("")
                       CASE_30("")
                       CASE_31("")
                       CASE_32("")
                       CASE_33("")
                       CASE_34("")
                       CASE_35("")
                       CASE_36("")
                       CASE_37("")
                       CASE_38("")
                       CASE_39("")
                       CASE_40("")
                       break;


                CASE_END


}


//******************************************************************************************
//man start
//&uart_listener Processes
//
//   -   This function enables uart listener processes
//
//Carlos A. Perez
//man end
//******************************************************************************************
void start_listen_thread()
{
      WRITE_STRING("\n\r");
      WRITE_STRING("Starting UART Processes\n\r");
      int ret, i, j;

        for( i = 0; i < NTHREADS; i++ )
        {
            switch (i)
            {
               case 1:
               for(j=0;j<=20;j++){WRITE_STRING("..");delay_ms(100);}
               WRITE_STRING("\n\r");
               ret = pthread_create( &ids[i], NULL, uart1_listener_process,(void *)(i));
               if( ret != 0 ){ WRITE_STRING("Error: uart1 RCV Protocols - RUNNING\n\r");}
               else{WRITE_STRING("STARTING...UART1 Listener Event- OK\n\r");}
               break;

               case 2:
               for(j=0;j<=20;j++){WRITE_STRING("..");delay_ms(100);}
               WRITE_STRING("\n\r");
               ret = pthread_create( &ids[i], NULL, uart2_listener_process,(void *)(i));
               if( ret != 0 ){ WRITE_STRING("Error: uart2 RCV Protocols - RUNNING\n\r");}
               else{WRITE_STRING("STARTING...UART2 Listener Event- OK\n\r");}
               break;

               case 3:
               for(j=0;j<=20;j++){WRITE_STRING("..");delay_ms(100);}
               WRITE_STRING("\n\r");
               ret = pthread_create( &ids[i], NULL, uart3_listener_process,(void *)(i));
               if( ret != 0 ){ WRITE_STRING("Error: uart3 RCV Protocols - RUNNING\n\r");}
               else{WRITE_STRING("STARTING...UART3 Listener Event- OK\n\r");}
               break;

               case 4:
               for(j=0;j<=20;j++){WRITE_STRING("..");delay_ms(100);}
               WRITE_STRING("\n\r");
               ret = pthread_create( &ids[i], NULL, uart4_listener_process,(void *)(i));
               if( ret != 0 ){ WRITE_STRING("Error: uart4 RCV Protocols - RUNNING\n\r");}
               else{WRITE_STRING("STARTING...UART4 Listener Event- OK\n\r");}
               break;

               default:
               break;
            }

        }

}

//******************************************************************************************
//man start
//&uart1_listener_process Processes
//
//   -   This function enables should be an endless background loop that
//       listens to the IO REC port.
//
//   -   EOL is key chracter by design. this flags allows an interrupt event to
//       feedback the end user and to restart the process again.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void uart1_listener_process(void *arg)
{
    uint64_t mark = clock_ms();
    struct timespec ts = { 0, (((int)(arg)+1)*FLASH_INTERVAL_MSCS)*1000L*1000L };
    do
    {

          start_process1:
          cmdChar1 = 0;
          cmdSize1 = -1;
          cmdLastChar1 = '\0';
          memset( strCmd_uart1, 0, sizeof( strCmd_uart1 ));
          for (;;)
          {
            if ( cmdChar1 = uart8_getchar( drv_uart8_1 ), cmdChar1 == -1 )continue;
            else if(cmdChar1 == BS)
            {
                if(strlen(strCmd_uart1)==0)continue;
                cmdSize1--;
                strCmd_uart1[cmdSize1 + 1] = '\0';
                continue;
            }
            else if(cmdChar1 == EOL)
            {
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING(strCmd_uart1);
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING("JBL-UART1>");
                goto start_process1;

            }
            else
            {
               cmdLastChar1 = cmdChar1;
               cmdSize1++;
               strCmd_uart1[cmdSize1] = cmdLastChar1;
            }

          }

        nanosleep(&ts,NULL);

    } while (elapsed_time_ms(mark) < RUNTIME_SCS*1000L);

    sprintf(strRemarks,"Exiting uart1 threads ( ~ %d scs ) ... \n", RUNTIME_SCS );
    WRITE_STRING(strRemarks);
}

//******************************************************************************************
//man start
//&uart2_listener_process Processes
//
//   -   This function enables should be an endless background loop that
//       listens to the IO REC port.
//
//   -   EOL is key chracter by design. this flags allows an interrupt event to
//       feedback the end user and to restart the process again.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void uart2_listener_process(void *arg)
{
    uint64_t mark = clock_ms();
    struct timespec ts = { 0, (((int)(arg)+1)*FLASH_INTERVAL_MSCS)*1000L*1000L };
    do
    {

          start_process2:
          cmdChar2 = 0;
          cmdSize2 = -1;
          cmdLastChar2 = '\0';
          memset( strCmd_uart2, 0, sizeof( strCmd_uart2 ));
          for (;;)
          {
            if ( cmdChar2 = uart8_getchar( drv_uart8_2 ), cmdChar2 == -1 )continue;
            else if(cmdChar2 == BS)
            {
                if(strlen(strCmd_uart2)==0)continue;
                cmdSize2--;
                strCmd_uart2[cmdSize2 + 1] = '\0';
                continue;
            }
            else if(cmdChar2 == EOL)
            {
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING(strCmd_uart2);
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING("JBL-UART2>");
                goto start_process2;
            }
            else
            {
                cmdLastChar2 = cmdChar2;
                cmdSize2++;
                strCmd_uart2[cmdSize2] = cmdLastChar2;
            }

          }

    } while (elapsed_time_ms(mark) < RUNTIME_SCS*1000L);

    sprintf(strRemarks,"Exiting uart2 threads ( ~ %d scs ) ... \n", RUNTIME_SCS );
    WRITE_STRING(strRemarks);
}

//******************************************************************************************
//man start
//&uart3_listener_process Processes
//
//   -   This function enables should be an endless background loop that
//       listens to the IO REC port.
//
//   -   EOL is key chracter by design. this flags allows an interrupt event to
//       feedback the end user and to restart the process again.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void uart3_listener_process(void *arg)
{
    uint64_t mark = clock_ms();
    struct timespec ts = { 0, (((int)(arg)+1)*FLASH_INTERVAL_MSCS)*1000L*1000L };
    do
    {

          start_process3:
          cmdChar3 = 0;
          cmdSize3 = -1;
          cmdLastChar3 = '\0';
          memset( strCmd_uart3, 0, sizeof( strCmd_uart3 ));
          for (;;)
          {
            if ( cmdChar3 = uart8_getchar( drv_uart8_3 ), cmdChar3 == -1 )continue;
            else if(cmdChar3 == BS)
            {
                if(strlen(strCmd_uart3)==0)continue;
                cmdSize3--;
                strCmd_uart3[cmdSize3 + 1] = '\0';
                continue;
            }
            else if(cmdChar3 == EOL)
            {
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING(strCmd_uart3);
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING("JBL-UART3>");
                goto start_process3;
            }
            else
            {
                cmdLastChar3 = cmdChar3;
                cmdSize3++;
                strCmd_uart3[cmdSize3] = cmdLastChar3;
            }

          }

    } while (elapsed_time_ms(mark) < RUNTIME_SCS*1000L);

    sprintf(strRemarks,"Exiting uart3 threads ( ~ %d scs ) ... \n", RUNTIME_SCS );
    WRITE_STRING(strRemarks);
}
//******************************************************************************************
//man start
//&uart4_listener_process Processes
//
//   -   This function enables should be an endless background loop that
//       listens to the IO REC port.
//
//   -   EOL is key chracter by design. this flags allows an interrupt event to
//       feedback the end user and to restart the process again.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void uart4_listener_process(void *arg)
{
    uint64_t mark = clock_ms();
    struct timespec ts = { 0, (((int)(arg)+1)*FLASH_INTERVAL_MSCS)*1000L*1000L };
    do
    {
          start_process4:
          cmdChar4 = 0;
          cmdSize4 = -1;
          cmdLastChar4 = '\0';
          memset( strCmd_uart4, 0, sizeof( strCmd_uart4 ));
          for (;;)
          {
            if ( cmdChar4 = uart8_getchar( drv_uart8_4 ), cmdChar4 == -1 )continue;
            else if(cmdChar4 == BS)
            {
                if(strlen(strCmd_uart4)==0)continue;
                cmdSize4--;
                strCmd_uart4[cmdSize4 + 1] = '\0';
                continue;
            }

            else if(cmdChar4 == EOL)
            {
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING(strCmd_uart4);
                SEND_RETURN_KEY_UART_MAIN
                WRITE_STRING("JBL-UART4>");
                goto start_process4;

            }
            else
            {
                cmdLastChar4 = cmdChar4;
                cmdSize4++;
                strCmd_uart4[cmdSize4] = cmdLastChar4;
            }
          }

    } while (elapsed_time_ms(mark) < RUNTIME_SCS*1000L);

    sprintf(strRemarks,"Exiting uart4 threads ( ~ %d scs ) ... \n", RUNTIME_SCS );
    WRITE_STRING(strRemarks);

}

//******************************************************************************************
//man start
//&get_command command
//
//   -   This function is responsible for splitting the user's input.
//   -   the CLI supports 5 parameters for syntax.
//   -   if none of the parameters/commands are recognized, it jumps into the sendStringCommand();
//
//Carlos A. Perez
//man end
//******************************************************************************************
int get_command()
{
        //*************Clear strCmd variables - Clears all commands parameters**************
        int j;
        wordCount = 0;
        char stateName[100];
        memset( strCmd1, 0, sizeof( strCmd1 ));
        memset( strCmd2, 0, sizeof( strCmd2 ));
        memset( strCmd3, 0, sizeof( strCmd3 ));
        memset( strCmd4, 0, sizeof( strCmd4 ));
        memset( strCmd5, 0, sizeof( strCmd5 ));

        //************Clone our original command and split it into an array*****************
        strcpy(stateName,strCmd);                                                     //copy the original command
        words[0] = strtok(stateName, " ");                                            //split the words into an array
        while(words[wordCount]!= NULL)
        {
            //ensure a pointer was found
            wordCount++;
            words[wordCount] = strtok(NULL, " "); //continue to tokenize the string
        }

        //If 'wordCount' remains '0' it means no command was sent.
        if(wordCount == 0)return 0;

        //**************Convert String Types and re-assign them to the Global Variable.******
        for(j = 0; j <= wordCount-1; j++)
        {
            if(j==0){sprintf(strRemarks,"%s", words[j]);strcpy(strCmd1,strRemarks);} //WRITE_STRING(strCmd1);}
            if(j==1){sprintf(strRemarks,"%s", words[j]);strcpy(strCmd2,strRemarks);} //WRITE_STRING(strCmd2);}
            if(j==2){sprintf(strRemarks,"%s", words[j]);strcpy(strCmd3,strRemarks);} //WRITE_STRING(strCmd3);}
            if(j==3){sprintf(strRemarks,"%s", words[j]);strcpy(strCmd4,strRemarks);} //WRITE_STRING(strCmd4);}
            if(j==4){sprintf(strRemarks,"%s", words[j]);strcpy(strCmd5,strRemarks);} //WRITE_STRING(strCmd5);}
        }
        //****************Start Supporting all your parameterize Commands.********************
        if (strcmp("set", strCmd1) == 0)return set_IO_port();
        if (strcmp("get", strCmd1) == 0)return get_IO_port();
        if (strcmp("check", strCmd1) == 0)return check_IO_port();
        return 0;
}

//******************************************************************************************
//man start
//&set command
//
//   -   This routine allows you to set a high or low parameter to the IO.
//       user can also type hex without the '0x' value.
//   -   Syntax errors will be advised by the set_IO_port_help() function.
//
//Carlos A. Perez
//man end
//******************************************************************************************

int set_IO_port_help() {
   WRITE_STRING("Usage: set [<port name>] [<high/low>]\n\r");
   WRITE_STRING("\t-port name: a string\n\r");
   WRITE_STRING("\t-0/1 Or HEX Value: a Int or Hex parameter\n\r");
   return 1;
}
int set_IO_port(void)
{
     int x,i, ErrorCommand = 0;
    //********************Check strCmds (parameters).***************************************
    if(strlen(strCmd2)<=0)ErrorCommand = set_IO_port_help();
    if(strlen(strCmd3)<=0)ErrorCommand = set_IO_port_help();
    CHECK_ERRORS(ErrorCommand);
    //***************************Value*****************************************************
    if (strcmp("high", strCmd3) == 0){memset( strCmd3, 0, sizeof( strCmd3 ));strCmd3[0] = '1';}
    if (strcmp("low", strCmd3) == 0){memset( strCmd3, 0, sizeof( strCmd3 ));strCmd3[0] = '0';}
    sscanf(strCmd3, "%x", &x);
    //***************Search for a valid PORT************************************************
    //**************Convert LOWER to UPPER for Consistancy***************************************
    //for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=65&&strCmd2[i]<=90)strCmd2[i]=strCmd2[i]+32;}  //Conversion from uppercase to lower case using c program
    for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=97&&strCmd2[i]<=122)strCmd2[i]=strCmd2[i]-32;}  //Conversion from lowercase to upper case using c program

    if (strcmp("J1_IO_0", strCmd2) == 0){return 1;}
    if (strcmp("J1_IO_1", strCmd2) == 0){return 1;}
    if (strcmp("GPIO_PA", strCmd2) == 0){return 1;}
   return 0;
}

//******************************************************************************************
//man start
//&verify_IO_Value command
//
//   -   This function is a 1-to-1 comparison. if you drive 1 and expect to see 1 use this
//       function. Otherwise see verify_IO_ValueInverted() function.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void verify_IO_Value(uint8_t getVal, uint8_t expectedVal)
{

    if( getVal != (expectedVal))
    {
      sprintf(strRemarks,"EXPECTED[0x%0X], RECIEVED[0x%0X] -[FAILED]\n\r", expectedVal, getVal );
      WRITE_STRING(strRemarks);

    }
    else
    {
       sprintf(strRemarks,"0x%08X -[PASSED]\n\r", getVal );
       WRITE_STRING(strRemarks);

    }

}

//******************************************************************************************
//man start
//&verify_IO_ValueInverted command
//
//   -   This function was provided because circuits not always compare values in the same
//       manner as they are driven. for instance. drive 0 to an LED may set it to a 1 state (pull-resistor).
//       or viseversal. This function is needed for hardware Engineers; they decide the expected value based
//       on their own researches.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void verify_IO_ValueInverted(uint8_t getVal, uint8_t expectedVal)
{

    if( getVal != (expectedVal))
    {
      sprintf(strRemarks,"EXPECTED[0x%0X], RECIEVED[0x%0X] -[PASSED]\n\r", expectedVal, getVal );
      WRITE_STRING(strRemarks);

    }
    else
    {
       sprintf(strRemarks,"0x%08X -[FAILED]\n\r", getVal );
       WRITE_STRING(strRemarks);

    }

}



//******************************************************************************************
//man start
//&get command
//
//   -   This routine allows you to set a high or low parameter to the IO.
//       user can also type hex without the '0x' value.
//   -   Syntax errors will be advised by the set_IO_port_help() function.
//   -   value and inverse are reverse logic. this needs to be decided between
//       the hardware engineer and software.
//
//Carlos A. Perez
//man end
//******************************************************************************************

int get_IO_port_help() {
   WRITE_STRING("Usage: get [<port name>]\n\r");
   WRITE_STRING("\t-port name: a string\n\r");
   return 1;
}
int get_IO_port(void)
{
     int ErrorCommand,i;
    //***********************Check strCmds (parameters).************************************
    if(strlen(strCmd2)==0)ErrorCommand = get_IO_port_help();
    CHECK_ERRORS(ErrorCommand);
    //***********************Search for a valid PORT****************************************
    //**************Convert LOWER to UPPER for Consistancy***************************************
    //for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=65&&strCmd2[i]<=90)strCmd2[i]=strCmd2[i]+32;}  //Conversion from uppercase to lower case using c program
    for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=97&&strCmd2[i]<=122)strCmd2[i]=strCmd2[i]-32;}  //Conversion from lowercase to upper case using c program

    if (strcmp("J1_IO_0", strCmd2) == 0){return 1;}
    if (strcmp("J1_IO_1", strCmd2) == 0){return 1;}
    if (strcmp("J1_IO_2", strCmd2) == 0){return 1;}
    if (strcmp("GPIO_PA", strCmd2) == 0){return 1;}
   return 0;
}

//******************************************************************************************
//man start
//&check command
//
//   -   This routine allows you to verify a desire state of an IO.
//       this funciton can be useful for the automation program while
//       (JABIL TEST) to set conditions and to check them.
//   -   value and inverse are reversed logic to support both states 0/1.
//
//Carlos A. Perez
//man end
//******************************************************************************************

int check_IO_port_help() {
   WRITE_STRING("Usage: check [<port name>] [<expected value>] [<verification type>]\n\r");
   WRITE_STRING("\t-port name: a string\n\r");
   WRITE_STRING("\t-0/1 Or HEX: expected value\n\r");
   WRITE_STRING("\t-value/inverse: verification type\n\r");
   return 1;
}
int check_IO_port(void)
{
     int x,ErrorCommand,i;
    //*****************************Check strCmds (parameters).********************************
    if(strlen(strCmd2)==0)ErrorCommand = check_IO_port_help();
    if(strlen(strCmd3)==0)ErrorCommand = check_IO_port_help();
    if(strlen(strCmd4)==0)ErrorCommand = check_IO_port_help();
    CHECK_ERRORS(ErrorCommand);
    //***********************************Value************************************************
    if (strcmp("high", strCmd3) == 0){memset( strCmd3, 0, sizeof( strCmd3 ));strCmd3[0] = '1';}
    if (strcmp("low", strCmd3) == 0){memset( strCmd3, 0, sizeof( strCmd3 ));strCmd3[0] = '0';}
    sscanf(strCmd3, "%x", &x);
    //***************************Search for a valid PORT*************************************
    //**************Convert LOWER to UPPER for Consistancy***************************************
    //for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=65&&strCmd2[i]<=90)strCmd2[i]=strCmd2[i]+32;}  //Conversion from uppercase to lower case using c program
    for(i=0;i<=strlen(strCmd2);i++){if(strCmd2[i]>=97&&strCmd2[i]<=122)strCmd2[i]=strCmd2[i]-32;}  //Conversion from lowercase to upper case using c program

    if (strcmp("J1_IO_0", strCmd2) == 0){return 1;}
    if (strcmp("J1_IO_1", strCmd2) == 0){return 1;}
    if (strcmp("GPIO_PA", strCmd2) == 0){return 1;}
   return 0;
}

//******************************************************************************************
//man start
//&get_io_state command
//
//   -   This routine displays the IO state (high/low) in Hex format
//
//Carlos A. Perez
//man end
//******************************************************************************************
void get_io_state(uint8_t getVal)
{

      sprintf(strRemarks,"RECIEVED[0x%0X]\n\r", getVal );
      WRITE_STRING(strRemarks);

}

//******************************************************************************************
//man start
//&verify_io_state command
//
//   -   This routine verifies the active state vs. a desire state typed
//       by the end user. this function features: value and invert.
//   -   value - compares the actual vs. the desire string (0,1, hex)
//   -   invert- compares the oposite state of the current state.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void verify_io_state(uint8_t getVal, uint8_t expectedVal)
{
    //Value
    if (strcmp("value", strCmd4) == 0)verify_IO_Value(getVal, expectedVal);
    if (strcmp("invert", strCmd4) == 0)verify_IO_ValueInverted(getVal, expectedVal);
}



//******************************************************************************************
//man start
//&compare_string(char *first, char *second)
//
//   -   This routine turnes a 0 or -1
//   -   it compares two char * pointers (strings).
//
//Carlos A. Perez
//man end
//******************************************************************************************
int compare_string(char *first, char *second)
{
   while(*first==*second)
   {
      if ( *first == '\0' || *second == '\0' )
         break;

      first++;
      second++;
   }
   if( *first == '\0' && *second == '\0' )
      return 0;
   else
      return -1;
}


//******************************************************************************************
//man start
//&show_ver()
//
//   -   This routine should be improve at a later version.
//   -   a nice feature will be to have elipcisis parameters
//   -   for now the information is captured from the Uart software panel
//
//Carlos A. Perez
//man end
//******************************************************************************************
void show_ver()
{

      char version[5] = {"2.0.1"};

      //WRITE_STRING("UART configuration: %d bps, %s parity, %d databits, %d stopbits\n", BAUDRATE, parstr[PARITY], DATABITS, STOPBITS);
      WRITE_STRING("\n\rJabil Inc. FPGA Operating System  Software\n\r");
      WRITE_STRING("Development support: http://www.jabil.com\n\r");
      WRITE_STRING("Copyright (c) 2012, Jabil Inc. All rights reserved\n\r");
      WRITE_STRING("The copyrights to certain works contained herein are owned by\n\r");
      WRITE_STRING("Raytheon and are used and distributed under Raytheon's permission.\n\r");
      WRITE_STRING("Software: \n\r");
      WRITE_STRING("UART BAUDRATE:          19200\n\r");
      WRITE_STRING("UART PARITY:            NONE\n\r");
      WRITE_STRING("UART DATABITS:          8\n\r");
      WRITE_STRING("UART STOP BITS:         1\n\r");
      WRITE_STRING("UART HANDSHAKE:         NONE\n\r");
      WRITE_STRING("UART SOFTWARE VERSION:  ");
      WRITE_STRING(version);
      WRITE_STRING("\n\r");
      WRITE_STRING("MEMORY:                 BLOCK RAM - 128KB (32K x 32 BIT)");
      WRITE_STRING("\n\r");
      WRITE_STRING("\n\r");
      WRITE_STRING("\n\r");

}

//******************************************************************************************
//man start
//&exit_prog()
//
//   -   This routine exits the program. if programmed is reset then it will
//       start from the main() funciton.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void exit_prog()
{
    WRITE_STRING("END OF PROGRAM\n\r");
    exit(0);
}

//******************************************************************************************
//man start
//&unknown_command()
//
//   -   This routine provides feed back to the end user.
//       the function was defiend at the SWITCH_#case.
//   -   if no phrase is recognized the MACRO will execute this function.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void unknown_command() {

    uart_active = is_uart_in_use(0);
    if( uart_active == MAIN_UART)
    {
       WRITE_STRING("\n\r Error: Unknown Command[");
       WRITE_STRING(strCmd);
       WRITE_STRING("]\n\r");
       WRITE_STRING("JBL>");
    }

}

//******************************************************************************************
//man start
//&set_uart_unused()
//
//   -   This routine uses bitwise to unset a bitmap.
//       bitmaps are use to decide on which UART is being use.
//
//Carlos A. Perez
//man end
//******************************************************************************************
int set_uart_unused(int uart_instance)
{
    uart_in_use = uart_in_use & ~(1<<uart_instance);
}
//******************************************************************************************
//man start
//&is_uart_in_use()
//
//   -   This routine returns the ACTIVE UART instance running the CLI
//
//Carlos A. Perez
//man end
//******************************************************************************************
int is_uart_in_use(int uart_instance)
{
    return uart_in_use & 1<<uart_instance;
}
//******************************************************************************************
//man start
//&flip_uart_use_state()
//
//   -   This routine defines the instance of the UART device to ACTIVE.
//
//Carlos A. Perez
//man end
//******************************************************************************************
void flip_uart_use_state(int uart_instance)
{
    uart_in_use = uart_in_use ^ 1<<uart_instance;
}









