#ifndef _CONFIG_H
#define _CONFIG_H

/*
 * define constants and pin layout
 * Pumpkraft: 60ml / s
 */

// PIN LAYOUT

// RTC connectors
#define RTC_SCL A5
#define RTC_SDA A4
#define RTC_VCC A3
#define RTC_GND A2

// sensors and motor
#define ECHO_TRIGGER 7
#define ECHO 6
#define LED_REFILL 3
#define RELAIS_TRIGGER 2
#define SENSOR 5
#define MANUAL_TRIGGER 4
// PINS 8 – 13 are reserved for the display

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

#endif
