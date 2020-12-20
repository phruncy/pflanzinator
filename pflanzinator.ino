/**
 * planzuino v0.1
 * @author Franziska Schneider
 * 
 * circuit based on the Garduino by Luke Iseman: https://www.instructables.com/Garduino-Gardening-Arduino/
 * distance sensor code by Ardutronix: https://ardutronix.de/wasserstandssensor-selber-bauen/
 * 
 */

#define DEBUG
#include "RTClib.h"
#include "config.h"

RTC_DS3231 rtc;

enum State {MEASURE, WATERING, REFILL, ERROR};
State state = State::MEASURE;

int wateringStart = 0;
unsigned long timerStart = 0;
int timeTableIndex  = 0;

void setup() {
  setPins();
  delay(100);
  Serial.begin(9600);
  // initialize timerStart
  if (! rtc.begin()) {
    Serial.println("Could not find RTC");
    Serial.flush();
    abort();
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power / please set time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  timerStart = millis();
}

void loop() {

    #ifdef DEBUG
      printTime();
    #endif
    
    switch(state) {
        case State::MEASURE:
            if (!hasWater()) {
                state = State::REFILL;
                break;
            }
            digitalWrite(LED_REFILL, LOW);
            // startWatering if schedule says so or if manually triggered
            if (digitalRead(MANUAL_TRIGGER) || isWateringScheduled()) {
                state = State::WATERING;
                wateringStart = millis();
            }
            break;
        case State::WATERING:
            waterRoutine();
            break;
        case State::REFILL:
            digitalWrite(LED_REFILL, HIGH);
            break;
        case State::ERROR:
            break;
    }
}

void waterRoutine()
{
    int wateringTime = millis() - wateringStart;
    digitalWrite(RELAIS_TRIGGER, LOW);
    if (wateringTime >= WATERING_DURATION) {
        digitalWrite(RELAIS_TRIGGER, HIGH);
        state = State::MEASURE;
    }
}

bool isWateringScheduled() 
{
  return false;
}

// measure the distance from echo to surface to determine if there is still enough water in the tank
bool hasWater()  
{
    digitalWrite(ECHO_TRIGGER, LOW);
    delay(5);
    digitalWrite(ECHO_TRIGGER, HIGH);
    delay(10);
    digitalWrite(ECHO_TRIGGER, LOW);
    long echoDuration = pulseIn(ECHO, HIGH);
    long waterDistance = (echoDuration / 2) * 0.03432;
    if (waterDistance >= 500 || waterDistance <= 0) {
        Serial.write("Fehler: Kein gültiger Messwert. ");
        Serial.print(waterDistance);
        Serial.println('\n');
        return false;
    } 
    float percentage = 100 - (((waterDistance - ECHO_OFFSET) * 100) / TANK_HEIGHT);
    return percentage >= MIN_WATER;
}

// attach all pins according to config.h
void setPins() 
{
  pinMode(RTC_SCL, INPUT);
  pinMode(RTC_SDA, INPUT);
  pinMode(RTC_VCC, OUTPUT);
  digitalWrite(RTC_VCC, HIGH);
  pinMode(RTC_GND, OUTPUT);
  digitalWrite(RTC_GND, LOW);
  pinMode(ECHO_TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_REFILL, OUTPUT);
  pinMode(RELAIS_TRIGGER, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT);
  digitalWrite(RELAIS_TRIGGER, HIGH);
  digitalWrite(LED_REFILL, LOW);
}

// just for debugging
void printTime() 
{
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ,");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.println();
}
