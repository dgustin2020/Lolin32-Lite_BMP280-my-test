/*
Board_Status_Led_Tbl.h
*/
#ifndef BOARD_STATUS_LED_TBL_H
#define BOARD_STATUS_LED_TBL_H
#include <my_define.h>

#define LED_BLINK_ON              FALSE
#define LED_BLINK_OFF             TRUE
#define LED_BLINK_END_SEQUENZA    0

extern const T_Lclk* status_led_tbl[];

extern const T_Lclk LED_STATUS_OFF[];
extern const T_Lclk LED_STATUS_ON[];
extern const T_Lclk LED_STATUS_WIFI_CONN[];
extern const T_Lclk LED_STATUS_NTP_CONN[];
extern const T_Lclk LED_STATUS_WIFI_ERR[];
extern const T_Lclk LED_STATUS_NTP_ERR[];
#endif