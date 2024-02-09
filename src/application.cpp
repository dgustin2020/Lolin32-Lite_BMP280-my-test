/*
application.cpp
*/
#include <itr_time.h>
//#include <Board_Def/board_def.h>

extern void main_init(void);
extern void main_loop(void);
extern void itr_1000_run(void);
/*****/ 
void setup() {
  Serial.begin(115200);
  /* init itr al 1/1000 */
  itr_time_init(itr_1000_run, TIMER_1000, PRESCALER_TIMER_1000, TIC_TIMER_1MSEC);
  /* init main loop*/
  main_init();//goto inizializza vari task
}


/****/
void loop() {
  /* mia gestione e tempo al millisecondo*/
  if (itr_time_scaduto()){//test se scaduto mSec x gestione procedure ad ogni mSec
    /*gestione procedure ad ogni mSec */
    itr_time_scaduto_clear(); //clear flag
    main_loop();              //chiamta procedura indicizzata ad ogni ,illisecondo
  }  
}

#if 0

    // gestione ricezione seriale su USB
    //usb_rs232_rx_task();
    // gestione ricezione seriale su bluetooth
    //blue_rs232_rx_task();  
    // genera main_clock 
    #if 0
    if (Main_Base_Offset.segno)
      Main_Lclk = Base_Lclk + Main_Base_Offset.value;
    else 
      Main_Lclk = Base_Lclk - Main_Base_Offset.value;
    #endif


    #if 0
    if (Serial.available()) {
      SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
      Serial.write(SerialBT.read());
    }
    #endif
  }
  #endif


