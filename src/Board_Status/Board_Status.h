/*
board_status.h
*/
#ifndef BOARD_STATUS_H
#define BOARD_STATUS_H

/* stati del led */
#define BOARD_STAT_INIT         0 // accensione
#define BOARD_STAT_ON           1 // se tutto OK
#define BOARD_STAT_WIFI_CONN    2 // tentativo connessione WiFi
#define BOARD_STAT_NTP_CONN     3 // tentativo connessione server NTP
#define BOARD_STAT_WIFI_ERR     4 // connnessione wifi non riuscita
#define BORAD_STAT_NTP_ERR      5 // connessione server ntp non riuscita

extern void     board_status_init(uint8_t pin_led, uint8_t status, T_Lclk time);
extern void     board_status_run(void);
extern void     board_status_change(uint8_t stat);
extern uint8_t  board_status_get(void);
extern uint8_t  board_status_get_tbl_len(void);
#endif