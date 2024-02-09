/*
board_status.cpp
gestione del blink led di stato
in relazione allo stato della board

*/
#include "Board_Status_Led_Tbl.h"
#include <Var_Common/Var_Common.h>

const T_Lclk* status_led_tbl[] = {
  LED_STATUS_OFF,           // 00
  LED_STATUS_ON,            // 01
  LED_STATUS_WIFI_CONN,//   // 02
  LED_STATUS_NTP_CONN,      // 03
  LED_STATUS_WIFI_ERR,      // 04
  LED_STATUS_NTP_ERR        // 05
};
#define N_MIN_BOARD_STAT        0
#define N_MAX_BOARD_STAT        5

static uint8_t status_flag;
static uint8_t status_flag_old;


/****/
void board_status_init(uint8_t pin_led, uint8_t status, T_Lclk time)
{
  if (status > N_MAX_BOARD_STAT)
    status = N_MIN_BOARD_STAT;
  status_flag = status;
  status_flag_old = status ;
  Led_Status.task_init(pin_led, (T_Lclk*)status_led_tbl[status_flag], time);
  Led_Status.task_disable();//disabilita task
}

/****/
void board_status_run(void)
{
  if (Led_Status.task_status()){//test se abilitato task
    if (Led_Status.task_run(Base_Lclk)){//test se reload sequenza blink
      if (status_flag != status_flag_old){//reload, verifica se cambiare sequenza
        status_flag_old = status_flag;//cambio sequenza
        Led_Status.task_change((T_Lclk*)status_led_tbl[status_flag], Base_Lclk);
      }
    }
  }

#if 0
  bool reload_sequenza;
  reload_sequenza = Led_Status.task_run(Base_Lclk);
  if (status_flag != status_flag_old){
    if (reload_sequenza){
      status_flag_old = status_flag;
      Led_Status.task_change((T_Lclk*)status_led_tbl[status_flag], Base_Lclk);
    }
  }

  if (status_flag == status_flag_old){
    Led_Status.task_run(Base_Lclk);
  }
  else {
    status_flag_old = status_flag;
    Led_Status.task_change((T_Lclk*)status_led_tbl[status_flag], Base_Lclk);
  }
#endif

}

/****/
void board_status_change(uint8_t stat)
{
  if (stat > N_MAX_BOARD_STAT)
    stat = N_MIN_BOARD_STAT;
  status_flag = stat;
}

/****/
uint8_t board_status_get(void)
{
  return status_flag;
}

/****/
uint8_t  board_status_get_tbl_len(void)
{
  return N_MAX_BOARD_STAT;
}
