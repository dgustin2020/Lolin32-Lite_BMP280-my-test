/*
board_status_led_tbl.c
*/
#include <my_define.h>
#include "Board_Status_Led_Tbl.h"

/* led sempre spento*/
const T_Lclk LED_STATUS_OFF[] = {
  0,LED_BLINK_OFF,
  LED_BLINK_END_SEQUENZA,0
};
/* led sempre acceso*/
const T_Lclk LED_STATUS_ON[] = {
  0,LED_BLINK_ON,
  LED_BLINK_END_SEQUENZA,0
};
/* led blink 50%, rate 500mSec*/
const T_Lclk LED_STATUS_WIFI_CONN[] = {
  300,LED_BLINK_ON,             // on x 500mS
  300,LED_BLINK_OFF,            //off x 500mS
  LED_BLINK_END_SEQUENZA,0
};
/* led blink 50%, rate 500mSec*/
const T_Lclk LED_STATUS_NTP_CONN[] = {
  150,LED_BLINK_ON,             // on x 500mS
  150,LED_BLINK_OFF,            //off x 500mS
  LED_BLINK_END_SEQUENZA,0
};
/****/
const T_Lclk LED_STATUS_WIFI_ERR[] = { // 2 blink led
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  1500,LED_BLINK_OFF,           //off x 900mS
  LED_BLINK_END_SEQUENZA,0
};
/****/
const T_Lclk LED_STATUS_NTP_ERR[] = { // 4 blink led
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  1500,LED_BLINK_OFF,           //off x 900mS
  LED_BLINK_END_SEQUENZA,0
};
