/*
main.cpp
*/
/*
main.cpp
*/
#include <Var_Common/Var_Common.h>
#include <My_Task_Gest.h>
#include <wifi/wifi_gest.h>
#include "Gest_Sensor/gest_sensor.h"
#include <Board_Def/board_def.h>
#include <Board_Status/Board_Status.h>
#include <usb_rs232.h>
#include <rs232/rs232_common.h>

extern void main_init(void);
extern void main_loop(void);

/****/
static void dummy(void){ return; }

/****/
static FnPtr        Pfn_Mainloop;     // dich. puntatore al programma principale
static uint8_t      Cnt_Mainloop;
static FnPtr        Afn_Mainloop[] = {// dich. array puntatori alle funzioni
  dummy,              //00
  board_status_run,   //01
  wifi_loop_run,      //02
  gest_sensor_run,    //03
  rs232_task,         //04
  0
};
#define MAINLOOP_MASK  0x07 //da modificare in funzione delle procedure usate

My_Task_Gest main_gest;//crea gestione  main_loop

/**** pubbliche ****/
/****/
void main_init(void)
{
  /* init led che indica lo stato della board */
  board_status_init(PIN_OUT_LED, BOARD_STAT_INIT, Base_Lclk);
  
  /* inizializza gestione loop wifi chiamato nel main loop*/
  wifi_loop_init();
  
  /* inizializza gestione loop snsore */
  gest_sensor_init();
  
  //init task rx rs232 via usb
  usb_rs232_init();
  
  /* inizializza loop del main */
  main_gest.init_and_run(0, MAINLOOP_MASK, Afn_Mainloop, &Base_Lclk);//inizializza main_loop
}

/***
 * main loop chiamato ad ogni milliSec generato da itr_1000
 * qui devo mettere tutte le procedure che devono essere gestite ad ogni milliSec
****/
void main_loop(void)
{
  Main_Lclk++;              // incre main clock ???????
  //gestione ricezione seriale su USB, ogni milliSec x non perdere char
  usb_rs232_rx_task();
  //passa alla prossima funzione e relativo run
  main_gest.next_and_run();
}


