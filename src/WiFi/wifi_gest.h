/*
wifi_gest.h
*/
#ifndef WIFI_GEST_H
#define WIFI_GEST_H

#define WIFI_CONN_LOOP_START      0
#define WIFI_CONN_LOOP_WAIT       1
#define WIFI_CONN_LOOP_ERR        2
#define WIFI_CONN_LOOP_OK         3
extern uint8_t wifi_loop_get_state(void);

#define WIFI_NTP_NONE             0
#define WIFI_NTP_WAIT             1
#define WIFI_NTP_ERR              2
#define WIFI_NTP_SYNC             3
extern uint8_t wifi_ntp_get_state(void);


extern void wifi_loop_init(void);
extern void wifi_loop_run(void);

extern void wifi_enable_ntp_clock_sync(void);//abilita gestione ntp server
extern void wifi_enable_web_server(void); //abilita gestione web server
extern char* wifi_conn_info(char* p_to);

#endif
