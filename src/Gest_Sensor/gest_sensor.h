/*
senosr.h
*/
#ifndef GEST_SENSOR_H
#define GEST_SENSOR_H


#define SENSOR_DELAY_MEASURE  2000 //in Msec, ogni quanto lanciafe il task run

/* procedure */
extern void gest_sensor_init(void);
extern void gest_sensor_run(void);

#endif
