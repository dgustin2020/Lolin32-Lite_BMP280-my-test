/*
rs232-usb-common.cpp
*/

#include <Arduino.h>
#include <my_define.h>
#include <usb_rs232.h>
#include <blue_rs232.h>
#include <my_string.h>
#include <Var_Common/Var_Common.h>
#include "rs232_common.h"
#include "proto.h"

uint8_t rs232_rx_buff[200];
uint8_t rs232_tx_buff[200];

/* sviluppo*/

/* procedure comuni */
/*
task a tempo che verifica se ricevuta una stribgs oppure
se bisogna inviarne una nuov
*/
void rs232_task(void)
{
  //gestione rs232 via usb
  if (usb_rs232_test_rx_str()){//test se ricevuto striga
    //ricevuta stringa, estrai
    usb_rs232_get_rx_str((uint8_t*)&rs232_rx_buff);
    //qui bisogna gestire la stringa ricevuta in rs232_rx_buff

    proto_gest((char*)&rs232_rx_buff, COM_PORT_USB);
#if 0
    //per esempio rimando la stringa ricevuta
    char* p_char = my_string_copy_terminate((char*)&rs232_rx_buff,(char*)&rs232_tx_buff);
    *p_char++ = 'x';
    *p_char++ = CR;
    *p_char++ = MIO_NULL;
    usb_rs232_load_tx_my_string((uint8_t*)&rs232_tx_buff);//load stringa da tramettere
#endif    
  }

  if (usb_rs232_test_tx_str()){//test se ho una stringa da trasmetter
    usb_rs232_tx_string(); //ho la stringa, la invio serial.print
  }
/**************************************************************************************/
  //gestione rs232 via bluetooth
  if (blue_rs232_test_rx_str()){//test se ricevuto striga
    //ricevuta stringa, estrai
    blue_rs232_get_rx_str((uint8_t*)&rs232_rx_buff);
    //qui bisogna gestire la stringa ricevuta

    //per esempio rimando la stringa ricevuta
    char* p_char = my_string_copy_terminate((char*)&rs232_rx_buff,(char*)&rs232_tx_buff);
    *p_char++ = 'x';
    *p_char++ = CR;
    *p_char++ = MIO_NULL;
    blue_rs232_load_tx_my_string((uint8_t*)&rs232_tx_buff);//load stringa da tramettere
    
  }

  if (blue_rs232_test_tx_str()){//test se ho una stringa datrasmetter
    blue_rs232_tx_string(); //ho la stringa, la invio serialBT.print
  }


}
