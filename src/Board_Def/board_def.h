/*
board_def.h
*/
#ifndef   _BOARD_DEF
#define   _BOARD_DEF

#include <Lolin32_pin_define.h>

/* definizione pin usati */
#define PIN_OUT_LED              PIN_IO_22 // LED_BUILTIN
#define PIN_IN_ADC_BATT_VOLT     PIN_IN_34 // (battery voltage / 2)  --> partitore (100K+100K)
#define PIN_IN_BATT_CHARGE       PIN_IO_14 // Low = charge run / High = charge end

/*BATTERIA*/
/* in battery,
   calcolo rapporto partitore ingresso
   valore da moltiplicare alla lettura della tensione del ADC
   per avere la vera tensione di batteria
*/
#define BATT_R1_PART            100000 //in ohm
#define BATT_R2_PART            100000 //in ohm
#define BATT_VOLT_RATE          (1 / (BATT_R1_PART / (BATT_R1_PART + BATT_R2_PART)))

/*SENSORE BMP280*/
/* BMP280 temp/press sensor */
/*da usare con Wire.beginn(SDA,SCL,)*/
#define PIN_OUT_SENSOR_I2C_SCL      PIN_IO_04 // scl BMP280
#define PIN_IO_SENSORE_I2C_SDA      PIN_IO_00 // sda BMP280
#define BMP280_SENSORE_I2C_ADDR     0x76
#define BMP280_DELAY_RATE           2000 //in mec

/* per genrare timer a tempo */
#define CLOCK_CPU               CPU_CLK_FREQ //80000000 //80MHz
/**** timer ****/
//#define TIMER_0               0
//#define TIMET_1               1
#define TIMER_1000              2 //n� timer x generazione itr ogni 1 mSec
#define TIMER_SOUND             3 //n� timer x generare suono


#endif

