#ifndef _CONFIG_H
#define _CONFIG_H

/*
 * define constants and pin layout
 * Pumpkraft: 60ml / s
 */

// PIN LAYOUT
#define ECHO_TRIGGER 9
#define ECHO 8
#define LED_REFILL 10
#define RELAIS_TRIGGER 6
#define SENSOR 12
#define MANUAL_TRIGGER 11

// constants
#define TANK_HEIGHT 19
#define ECHO_OFFSET 0.8
#define MIN_WATER 20
#define WATERING_DURATION 10000

// soil values
#define DRY_SOIL 400
#define MIN_SOIL 100

// time in millis
#define HOURS_22 79200000
#define MINUTES_30 1800000
#define HOURS_72 259200000

#define WATERING_PERIODS 3


#endif
