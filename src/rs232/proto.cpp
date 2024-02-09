/*
proto.cpp
*/
//#include <header.h>


#include "proto.h"
#include <my_define.h>
#include <usb_rs232.h>
#include <blue_rs232.h>
#include <my_string.h>
#include "WiFi/wifi_gest.h"
#include <bmp280-driver.h>

static char* metti_cmd(char cmd, char* p_to);
static char* string_cmd_cfm_ok(char cmd, char* p_to);
static char* genera_device_info_short(char cmd, char type, char* p_to);
static char* genera_device_info_long(char cmd, char type, char* p_to);
static char* genera_wifi_info(char cmd, char type, char* p_to);

static char proto_device_info_long(void);
static char proto_device_info_short(void);
static char proto_wifi_info(void);
static char proto_on_off_bmp230(void);

static char_FnPtr         P_Funct;      // dich. puntatore al programma principale
static const char_FnPtr   A_Funct[] = { // dich. array puntatori alle funzioni
  proto_device_info_long,         //  0 'a' P_DEVICE_INFO_LONG
  proto_device_info_short,        //  1 'b' P_DEVICE_INFO_SHORT
  proto_wifi_info,                //  2 'c' P_WIRELESS_DISCONNECT
  proto_on_off_bmp230,            //  3 'd' P_RECORD_REGOLA
  0,                  //  4 'e' P_WIRELESS_TO_RS232
  0,                //  5 'f' P_WIRELESS_SYNC_CLOCK
  0,                   //  6 'g' P_WIRELESS_SYNC_GO
  0,             //  7 'h' P_WIRELESS_TX_CLOCK_FOR_SYNC
  0,    //  8 'i' P_WIRELESS_TX_BORA_MODO_RUN
  0,              //  9 'j' P_WIRELESS_CI_SONO_BORA
  0,               // 10 'k' P_WIRELESS_TX_CHANGE_VIEW
  0,                  // 11 'l' P_BORA_RACE_N_TRIAL 
  0,               // 12 'm' P_BORA_RACE_TRIAL_DATA
  0,          // 13 'n' P_WIRELESS_TX_AVE_MEDIA_RUN
  0,          // 14 'o' P_WIRELESS_TX_AVE_DISTA_RUN
  0,           // 15 'p' P_WIRELESS_TX_BORA_LEN_PRV
  0,                   // 16 'q' P_WIRELESS_TX_SWAP
  0,           // 16 'r' P_WIRELESS_TX_AVE_SET_VIEW
  0,         // 17 's' P_WIRELESS_TX_SET_GPS_TO_232
  0,           // 18 't' P_DEVICE_GOTO_UPGRADE_FIRM
  0
};
#define P_CMD_MIN 'A'//P_DEVICE_INFO_LONG
#define P_CMD_MAX 'D'//

static char   loc_cmd;
static char   loc_cpm;
static char*  proto_from = 0;
static char*  proto_to = 0;
static char   proto_str[200];//buffer dove generare le stringhe in formato proto



/********************************************
analizza la stringa ricevuta e passata
a seconda dei comandi, fa cose
com_port = 0 ---> rs232
com_port = 1 ---> wireless
Se tutto OK, esce con true
****/
bool proto_gest(char* p_dati, char com_port)
{
char invia_str = 0;;
char t;
  proto_from = p_dati;
  loc_cpm = com_port;
  proto_to = proto_str;
  if (*proto_from++ != P_PREAMBOLO){   //load & skip sepa
    proto_to = my_string_copy_terminate((char*)"Er preamble: ",proto_to);//scrivi "errore"
    proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
    *proto_to++ = P_END_STRING;//metti CR
    *proto_to = MIO_NULL;//metti fine stringa
    usb_rs232_load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
    return FALSE;                 //preambolo erraato, rispondo con errore
  }
  /* load comando */
  proto_from++;//skip separatore
  loc_cmd = *proto_from++;
  /* se minuscolo converti in maiuscolo*/
  if (loc_cmd > 'Z')
    loc_cmd -= ('a' - 'A');
  if ((loc_cmd >= (char)P_CMD_MIN) && (loc_cmd <= (char)P_CMD_MAX)){
    P_Funct = A_Funct[(loc_cmd - (char)P_CMD_MIN)];	// load address procedura indicizzata
    if (P_Funct()){  /* test se devo inviare una stringa */
      *proto_to++ = P_END_STRING;
      *proto_to = MIO_NULL;
      usb_rs232_load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
      return TRUE;
    }
    else {
      proto_to = my_string_copy_terminate((char*)"Er Flg: ",proto_to);//scrivi "errore"
      proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
      *proto_to++ = P_END_STRING;//metti CR
      *proto_to = MIO_NULL;//metti fine stringa
      usb_rs232_load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
      return FALSE;
    }
  }
  else {
    proto_to = my_string_copy_terminate((char*)"Er Cmd: ",proto_to);//scrivi "errore"
    proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
    *proto_to++ = P_END_STRING;//metti CR
    *proto_to = MIO_NULL;//metti fine stringa
    usb_rs232_load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
    return FALSE;
  }
}

/*****************************************************************************************/
/*****************************************************************************************/
/*********** RICEZIONE STRINGHE ***********************/
/****/
static char proto_device_info_long(void)
{
  proto_from++;//skip separatore
  if (*proto_from++ == P_REQUEST_TYPE){   //load & skip sepa
    //richiesta, invio info
    proto_to = genera_device_info_long(loc_cmd, P_CONFIRM_TYPE, proto_str);
    proto_from++; //skip sepa
    return TRUE;
  }
  return false;
}

/****/
static char proto_device_info_short(void)
{
  proto_from++;//skip separatore
  if (*proto_from++ == P_REQUEST_TYPE){   //load & skip sepa
    //richiesta, invio info
    proto_to = genera_device_info_short(loc_cmd, P_CONFIRM_TYPE, proto_str);
    proto_from++; //skip sepa
    return TRUE;
  }
  return false;
}

/****/
static char proto_wifi_info(void)
{
  proto_from++;//skip separatore
  if (*proto_from++ == P_REQUEST_TYPE){   //load & skip sepa
    //richiesta, invio info
    proto_to = genera_wifi_info(loc_cmd, P_CONFIRM_TYPE, proto_str);
    proto_from++; //skip sepa
    return TRUE;
  }
  return false;
}  

/****/
static char proto_on_off_bmp230(void)
{
  proto_from++;
  if (*proto_from++ == P_REQUEST_TYPE){    //ricevuta richiesta invio record
    proto_from++;
    if (*proto_from == '0')
      bmp280_task_stop();       //disabilita task lettura parametri temp.sens
    else 
      bmp280_task_start();       //disabilita task lettura parametri temp.sens
    proto_to = string_cmd_cfm_ok(loc_cmd, proto_to);
    *proto_to++ = *proto_from;
    return TRUE;
  }
  else {
    return FALSE;
  }
}
/**************************************************************************************
***************************************************************************************/
/*
* mette il comando con il char preambolo
*/
static char* metti_cmd(char cmd, char* p_to)
{
  *p_to++ = P_PREAMBOLO;
  *p_to++ = P_SEPARA;
  *p_to++ = cmd;
  *p_to++ = P_SEPARA;
  return p_to;
}
/*
 * mette conferma al comando ricevuto
*/
static char* string_cmd_cfm_ok(char cmd, char* p_to)
{
  p_to = metti_cmd(cmd, p_to);
  *p_to++ = P_CONFIRM_TYPE;
  *p_to++ = P_SEPARA;
  return p_to;
}
/* 
 * genera la stringa con il tipo e il numereo seriale del crono 
*/
static char* genera_device_info_short(char cmd, char type, char* p_to)
{
char*   p_from;
char cnt_char;
/* metti inizio stringa */
  p_to = metti_cmd(cmd, p_to);
/* metti type */
  *p_to++ = type;
  *p_to++ = P_SEPARA;
/* metti nome strumento */
  p_from = (char*)"NOME_PROGRAMMA";
   cnt_char = 0;
   while (*p_from != MIO_NULL){
     *p_to++ = *p_from++;
     cnt_char++;
   }
  *p_to++ = P_SEPARA;
  return p_to;//esci con ptr al posto giusto
}
/****
genera la stringa con il tipo e il numereo seriale del crono e la versione
****/
static char* genera_device_info_long(char cmd, char type, char* p_to)
{
  p_to = genera_device_info_short(cmd, type, p_to);
  *p_to++ = P_SEPARA;
  /* metti versione firmware */
  p_to = my_string_copy_terminate((char*)"PROG_INFO_RS232_TXT", p_to);
  p_to--;
  p_to--;
  return p_to;
}
/*
genera una stringa con info wifi
*/
char* genera_wifi_info(char cmd, char type, char* p_to)
{
char*   p_from;
  /* metti inizio stringa */
  p_to = metti_cmd(cmd, p_to);
  /* metti type */
  *p_to++ = type;
  *p_to++ = P_SEPARA;
  /* qui metti dati relativi al wifi */
  p_to = wifi_conn_info(p_to);

  return p_to;
}
