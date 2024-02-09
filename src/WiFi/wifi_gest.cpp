/*
wifi_gest.cpp
*/
#include <my_define.h>
#include <MyWifi.h>
#include <My_Task_Gest.h>
#include <Var_Common/Var_Common.h>
#include <Board_Status/Board_Status.h>
#include <lib_clock_long.h>
#include "wifi_gest.h"
#include <usb_rs232.h>
#include <my_string.h>
#include <Abilita_Debug.h>

/********************************************************************************************/
/* private*/

static My_Task_Gest loc_loop;//crea task locale

#define WIFI_DUMMY                0
static void dummy(void);
#define WIFI_ERR_INIT             1
static void wifi_err_init(void); 
#define WIFI_ERR_WAIT             2
static void wifi_err_wait(void);
#define WIFI_SET_PARAM            3
static void wifi_set_param(void);
#define WIFI_CONN_BEGIN           4
static void wifi_conn_begin(void);
#define WIFI_CONN_WAIT            5
static void wifi_conn_wait(void);
#define WIFI_CONN_WAIT_OK         6
static void wifi_conn_wait_ok(void);

#define WIFI_CONN_NTP_BEGIN       7
static void wifi_conn_ntp_begin(void);
#define WIFI_CONN_NTP_WAIT        8
static void wifi_conn_ntp_wait(void);

#define WIFI_WEB_SERVER_START     9
static void wifi_web_server_start(void);
#define WIFI_WEB_SERVER_RUN       10
static void wifi_web_server_run(void);

#define WIFI_CONN_NTP_ERR_INIT    11
static void wifi_ntp_err_init(void);
#define WIFI_CONN_NTP_ERR_WAIT    12
static void wifi_ntp_err_wait(void);



/*****************************************************************************************/
/* tabella procedure chiamta dal tastk*/
static FnPtr        Afn_loc_loop[] = {// dich. array puntatori alle funzioni
  dummy,                  //00 WIFI_DUMMY
  wifi_err_init,          //01 WIFI_ERR_INIT
  wifi_err_wait,          //02 WIFI_ERR_WAIT
  wifi_set_param,         //03 WIFI_SET_PARAM
  wifi_conn_begin,        //04 WIFI_CONN_BEGIN
  wifi_conn_wait,         //05 WIFI_CONN_WAIT
  wifi_conn_wait_ok,      //06 WIFI_CONN_WAIT_OK

  wifi_conn_ntp_begin,    //07 WIFI_CONN_NTP_BEGIN
  wifi_conn_ntp_wait,     //08 WIFI_CONN_NTP_WAIT

  wifi_web_server_start,  //09 WIFI_CONN_OK
  wifi_web_server_run,    //10 WIFI_CONN_OK

  wifi_ntp_err_init,      //11 WIFI_CONN_NTP_ERR_INIT
  wifi_ntp_err_wait,      //12 WIFI_CONN_NTP_ERR_WAIT
  dummy,                  //12 WIFI_DUMMY
  0
};
#define WIFI_LOOP_MASK  0x0f //da modificare in funzione della quantità di procedure usate
static T_Lclk sub_timer;

static uint8_t wifi_loop_conn_state;
static uint8_t wifi_ntp_state;
static bool en_ntp_clock_sync;//true == abiltata sicronizzazione
static bool en_web_server;//true == abilitare web server


/********************************************************************************/
/* pubbliche */

/**/
uint8_t wifi_loop_get_state(void)
{  return wifi_loop_conn_state; }

/**/
uint8_t wifi_ntp_get_state(void)
{  return wifi_ntp_state; }

/* start sync clock via ntp*/
void wifi_enable_ntp_clock_sync(void)
{ en_ntp_clock_sync = true; }

/* start sync clock via ntp*/
void wifi_enable_web_server(void)
{ en_web_server = true; }

/* start gestione wifi da chimare in setup */
void wifi_loop_init(void)
{
  loc_loop.init(WIFI_SET_PARAM, WIFI_LOOP_MASK, Afn_loc_loop, &Base_Lclk);
  en_ntp_clock_sync = false;
  en_web_server = false;
  wifi_loop_conn_state = WIFI_CONN_LOOP_START;
  wifi_ntp_state = WIFI_NTP_NONE;
}

/* gestione wifi, chiamata da main_loop*/
void wifi_loop_run(void)
{
  if (loc_loop.task_active){
    loc_loop.run();
  }
}

/************************************************/
/* private */
/****/
static void dummy(void) 
{ 
  if (Base_Lclk > loc_loop.loc_time){
    loc_loop.loc_time += 1000;
    #ifdef DEBUG_WIFI
    Serial.print("Dummy ");
    Serial.println(Base_Lclk);
    #endif
  }
}
/***/
static void wifi_err_init(void)
{
  wifi_loop_conn_state = WIFI_CONN_LOOP_ERR;
  board_status_change(BOARD_STAT_WIFI_ERR);//cambia blink led
  loc_loop.set_idx(WIFI_ERR_WAIT);
}
/****/
static void wifi_err_wait(void)
{
  if (Base_Lclk > loc_loop.loc_time){
    loc_loop.loc_time += 1000;
    #ifdef DEBUG_WIFI
    Serial.println("WiFi Error");
    #endif
  }
}
/****/
static void wifi_set_param(void)
{
  wifi_loop_conn_state = WIFI_CONN_LOOP_WAIT;
  board_status_change(BOARD_STAT_WIFI_CONN);//cambia blink led
  WiFi.mode(WIFI_STA);
  #ifdef DEBUG_WIFI
  Serial.print("Local_IP: ");
  Serial.print(local_IP);
  Serial.print(" - Gateway: ");
  Serial.print(gateway);
  Serial.print(" - subnet: ");
  Serial.print(subnet);
  Serial.print(" - DNS1: ");
  Serial.print(primaryDNS);
  Serial.print(" - DNS2: ");
  Serial.println(secondaryDNS);
  #endif
  if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    /* errore di configurazione, passo alla segnalazione errore wifi*/
    loc_loop.set_idx(WIFI_ERR_INIT);
    #ifdef DEBUG_WIFI
    Serial.println("Failed to configure Local_IP");
    #endif
    return;
  }
  /* configurazione OK, passo alla connessione */
  loc_loop.loc_time = Base_Lclk + 1000;//set un timer per coreografia
  loc_loop.set_idx(WIFI_CONN_BEGIN);
}
/****/
static void wifi_conn_begin(void)
{
  if (Base_Lclk > loc_loop.loc_time){//test se scaduto timeout di coreografia
    #ifdef DEBUG_WIFI
    Serial.print("Connecting to WiFi -> ");
    Serial.print("SSID:");
    Serial.print(ssid);
    Serial.print(" - Password:");
    Serial.println(password);
    #endif
    /* prova la connessione */
    WiFi.begin(ssid, password);
    loc_loop.set_idx(WIFI_CONN_WAIT);//passa ad attendere risultato connessione
    loc_loop.loc_time = Base_Lclk + 10000;//set timeout connessione
    sub_timer = loc_loop.loc_time + 200;//set timer per segnalazione su seriale
  }
}
/****/
static void wifi_conn_wait(void)
{
  /*test se connessione avvenuta*/
  if (WiFi.status() == WL_CONNECTED) {
    wifi_loop_conn_state = WIFI_CONN_LOOP_OK;
    /* connessione ok*/
    #ifdef DEBUG_WIFI
    Serial.println();
    Serial.print("Local IP trovato:");
    Serial.println(WiFi.localIP());
    #endif
    loc_loop.set_idx(WIFI_CONN_WAIT_OK);//passa attesa fine connessione con 
    loc_loop.loc_time = 0;
    return;
  }
  /*test rimer print su seriale*/
  #ifdef DEBUG_WIFI
  if (Base_Lclk > sub_timer){
    /* scaduto sub_timer, emetto segnalazione su seriale*/
    sub_timer += 200;
    Serial.print('.');
    return;
  }
  #endif
  /* test timeout connessione*/
  if (Base_Lclk > loc_loop.loc_time){//test se scaduto timeout connesione
    //scaduto, passa a segnala re errore
    loc_loop.set_idx(WIFI_ERR_INIT);
    #ifdef DEBUG_WIFI
    Serial.println("Wifi Begin Timeout Error");
    #endif
    return;
  }
}
/****/
static T_Lclk wifi_conn_Lclk = 0;
static void wifi_conn_wait_ok(void)
{
  if (en_ntp_clock_sync){
    /* aspetta comando per passare alla sincronizzazione del clock via NTP*/
    en_ntp_clock_sync = false;
    loc_loop.set_idx(WIFI_CONN_NTP_BEGIN);//passa alla connessione al server NTP
    loc_loop.loc_time = 0;
    return;
  }
  if (en_web_server){
    /* aspetta comando per passare alla gestione del web server */
    en_web_server = false;
    loc_loop.set_idx(WIFI_WEB_SERVER_START);//passa alla connessione al server NTP
    loc_loop.loc_time = 0;
    return;
  }

    if (Base_Lclk > wifi_conn_Lclk){
      wifi_conn_Lclk = Base_Lclk + 5000;
      if (WiFi.status() == WL_CONNECTED) {
        #ifdef DEBUG_WIFI
        //Serial.print("WiFi Connesso -> ");
        //Serial.println(WiFi.localIP());
        usb_rs232_load_tx_my_string((uint8_t*)"WiFi Connesso\r");//load stringa da tramettere
        #endif
      }
      else {
        #ifdef DEBUG_WIFI
        Serial.println("WiFi non connesso");
        #endif
        //torna a inizio connessione
        loc_loop.set_idx(WIFI_SET_PARAM);
        en_ntp_clock_sync = false;
        en_web_server = false;
        wifi_loop_conn_state = WIFI_CONN_LOOP_START;
        wifi_ntp_state = WIFI_NTP_NONE;
      }
    }
}

/*
genera una stringa con informazioni su WiFi
*/
char* wifi_conn_info(char* p_to)
{
  if (WiFi.status() == WL_CONNECTED){
    p_to = my_string_copy_terminate((char*)"WiFi connect", p_to);
    p_to = my_string_copy_terminate((char*)" to ", p_to);
    p_to = my_string_copy_terminate((char*)ssid, p_to);
  }
  else {
    p_to = my_string_copy_terminate((char*)"WiFi not connect", p_to);
  }
  return p_to;
}
/**********************/
/**** gestione NTP ****/
/****/
static void wifi_conn_ntp_begin(void)
{
  wifi_ntp_state = WIFI_NTP_WAIT;
  board_status_change(BOARD_STAT_NTP_CONN);//cambia blink led
  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  #ifdef DEBUG_WIFI
  Serial.print("Initializing Date and Time from NTP Server: ");
  Serial.println(ntpServer);
  #endif
  loc_loop.loc_time = Base_Lclk + 10000;//set timeout connessione NTP
  sub_timer = Base_Lclk + 500;
  loc_loop.set_idx(WIFI_CONN_NTP_WAIT);// cambia stato

}
/****/
static void wifi_conn_ntp_wait(void)
{
  if (Base_Lclk > loc_loop.loc_time){
    /* scaduto timeou connessione NTP, passa a segnalare errore*/
    loc_loop.set_idx(WIFI_CONN_NTP_ERR_INIT);// cambia stato
    return;
  }
  /*attesa ricezione ora ntp*/
  struct tm timeinfo;
  /*test se ricevuta ora*/
  if(!getLocalTime(&timeinfo)){
    /*non ricevuta ora, aspetto*/
    #ifdef DEBUG_WIFI
    if (Base_Lclk > sub_timer){//invio '.' su seriale
      sub_timer += 500;
      Serial.print(".");
    }
    #endif
    return;
  }
  /* load ora ricevuta in formato T:Lclk */
  T_Lclk time_Lclk;
  time_Lclk = (timeinfo.tm_hour * L_ORA) + (timeinfo.tm_min * L_MIN) + (timeinfo.tm_sec * L_SEC);
  Main_Lclk = time_Lclk;
  #ifdef DEBUG_WIFI
  Serial.print("NTP time: ");
  Serial.print(time_Lclk);
  Serial.print("  Ora: ");
  Serial.print(Lclk_to_ore(Main_Lclk));
  Serial.print(":");
  Serial.print(Lclk_to_min(Main_Lclk));
  Serial.print(":");
  Serial.println(Lclk_to_sec(Main_Lclk));
  #endif
  /* cambia blink led per segnalare tutto OK*/
  wifi_ntp_state = WIFI_NTP_SYNC;
  board_status_change(BOARD_STAT_ON);
  loc_loop.loc_time = Main_Lclk;
  /* cambia stato*/
  loc_loop.set_idx(WIFI_WEB_SERVER_START);
}
/****/
static void wifi_ntp_err_init(void)
{
  wifi_ntp_state = WIFI_NTP_ERR;
  board_status_change(BORAD_STAT_NTP_ERR);//cambia blink led
  loc_loop.set_idx(WIFI_CONN_NTP_ERR_WAIT);
}
/****/
static void wifi_ntp_err_wait(void)
{
  #ifdef DEBUG_WIFI
  if (Base_Lclk > loc_loop.loc_time){
    loc_loop.loc_time += 1000;
    Serial.println("NTP Error");
  }
  #endif
}

/******************************/
/**** gestione web serever ****/
T_Lclk tmr_server_Lclk;
extern void web_server_start(void);
extern void web_server_run(void);
extern void web_server_restart(void);
/****/
static void wifi_web_server_start(void)
{
  /*
  start web server che deve essere caricato manualmente in SPIFF
  In "data" troviamo il codice per creare il web server
  Con PLATFORMIO - Build Filesystem Image creamo immagine del codice del web server
  Con PLATFORMIO - Upload Filesystem Image carichiamo l'immagine del web server da usare
  */
  web_server_start();
  tmr_server_Lclk = Main_Lclk + 10000;
  /* cambia stato*/
  loc_loop.set_idx(WIFI_WEB_SERVER_RUN);

}
/****/
static void wifi_web_server_run(void) 
{ 
  if (Main_Lclk >= loc_loop.loc_time){
    loc_loop.loc_time += 1000; 
    web_server_run();//aggiotna parametri web server
    #if 0
    Serial.print("Load NTP time OK: ");
    Serial.print("  Ora: ");
    Serial.print(Lclk_to_ore(Main_Lclk));
    Serial.print(":");
    Serial.print(Lclk_to_min(Main_Lclk));
    Serial.print(":");
    Serial.println(Lclk_to_sec(Main_Lclk));
    #endif
  }
}
