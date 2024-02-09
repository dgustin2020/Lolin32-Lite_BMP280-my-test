/*
my_sensor.cpp
*/
#include <Board_Def/board_def.h>
#include <bmp280-driver.h>
#include <Wire.h>
#include <Var_Common/Var_Common.h>
#include <Out_Pin.h>
#include "gest_sensor.h"
#include <Abilita_Debug.h>

Out_Pin Led_Sensor;
T_Lclk  Led_Sensor_Lclk;
/****/
void gest_sensor_init(void)
{
  Wire.begin(PIN_IO_SENSORE_I2C_SDA, PIN_OUT_SENSOR_I2C_SCL);//set pin da usare per cmp280
  if (!bmp280_task_init(PIN_IO_SENSORE_I2C_SDA, 
                        PIN_OUT_SENSOR_I2C_SCL, 
                        BMP280_SENSORE_I2C_ADDR,
                        BMP280_DELAY_RATE)
                        ){
    Serial.println("Problema con sensore BMO280");
  }       
  else { // init  comunicazione con temp.sens
    Led_Sensor.init(PIN_OUT_LED, FALSE);
    Led_Sensor_Lclk = 0;
    bmp280_task_stop();       //disabilita task lettura parametri temp.sens
  }
}


T_Lclk gest_sensor_Lclk = 0;
/****/
void gest_sensor_run(void)
{
  uint8_t temp;
  temp = bmp280_task_run(Base_Lclk);
  switch (temp){
    case BMP280_TASK_RESULT_WAIT:
    /*gestione blink led se non ttivo task board_status*/
      if (!Led_Status.task_status()){//non azione su led se task led staus abilitato
        if ( (Led_Sensor_Lclk > 0) && (Base_Lclk > Led_Sensor_Lclk) ){
          Led_Sensor.on();
          Led_Sensor_Lclk = 0;
        }      
      }
      break;

    case BMP280_TASK_RESULT_OK:
      /*gestione blink led se non attivo task board_status*/
      if (!Led_Status.task_status()){//non azione su led se task board staus abilitato
        Led_Sensor.off();
        Led_Sensor_Lclk = Base_Lclk + 100;
      }
      if (sensor_read.new_read){ //test se nuova lettura
        sensor_read.new_read = FALSE;
        #ifdef DEBUG_BMP280
        Serial.print(F("Temperature = "));
        Serial.print(sensor_read.temp);
        Serial.println(" *C");
        Serial.print(F("Pressure = "));
        Serial.print(sensor_read.press);
        Serial.println(" Pa");
        Serial.print(F("Approx altitude = "));
        Serial.print(sensor_read.alti); /* Adjusted to local forecast! */
        Serial.println(" m");
        Serial.println();  
        #endif
      }
      break;
    
    case BMP280_TASK_RESULT_ERR:
      if (Base_Lclk > gest_sensor_Lclk){
        gest_sensor_Lclk = Base_Lclk + 5000;
        Serial.print(temp);
        Serial.print("  ");
        Serial.println("Errore leggendo BMP280");
      }
      break;

    case BMP280_TASK_RESULT_STOP:
      if (Base_Lclk > gest_sensor_Lclk){
        gest_sensor_Lclk = Base_Lclk + 10000;
        Serial.print(temp);
        Serial.print("  ");
        Serial.println("Task BMP280 non abilitato");
      }
      break;

    case BMP280_TASK_RESULT_NOT_FOUND:
      if (Base_Lclk > gest_sensor_Lclk){
        gest_sensor_Lclk = Base_Lclk + 5000;
        Serial.print(temp);
        Serial.print("  ");
        Serial.println("BMP280 non trovato");
      }
      break;

    default:
    //  Serial.print(temp);
    //  Serial.print("  ");
    //  Serial.println("Problema default");
      break;
  }
}