/**
 * Messwert-Referenzen (komplette Flächer):  
 * Wasser: ca 630
 * sehr trockene Erde: ca 150
 * trockene Erde: ca. 300 - 350
 * feuchte Erde: 680 - 750
 * Erde direkt nach dem Gießen: ca 750
 * ----> gießen unter: 300
 * ----> NICHT gießen unter: 50 (Schutz vor Überschwemmung)
 */

#include "config.h"

enum State {MEASURE, WATERING, REFILL, ERROR};
State state = State::MEASURE;

unsigned long timeTable[3] = {MINUTES_30, MINUTES_30, HOURS_72};

int sensorValue = 500;
int wateringStart = 0;
unsigned long timerStart = 0;
int timeTableIndex  = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ECHO_TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_REFILL, OUTPUT);
  pinMode(RELAIS_TRIGGER, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(MANUAL_TRIGGER, INPUT);
  digitalWrite(RELAIS_TRIGGER, HIGH);
  digitalWrite(LED_REFILL, LOW);
  // initialize timerStart
  timerStart = millis();
}

void loop() {
    unsigned long timePassed = millis() - timerStart;
    Serial.write("Time passed in current counter: ");
    Serial.print(timePassed / 1000);
    Serial.write("schedule index: ");
    Serial.println(timeTableIndex);
//    Serial.write("Schedule nr: ");
//    Serial.println(timeTableIndex);
    switch(state) {
        case State::MEASURE:
            if (!hasWater()) {
                state = State::REFILL;
                break;
            }
            digitalWrite(LED_REFILL, LOW);
            if (digitalRead(MANUAL_TRIGGER) || timePassed > timeTable[timeTableIndex]) {
                state = State::WATERING;
                wateringStart = millis();
                timerStart = millis();
                timeTableIndex = (timeTableIndex + 1) % WATERING_PERIODS;
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
//    Serial.write("Watering: ");
//   Serial.println(wateringTime);
    if (wateringTime >= WATERING_DURATION) {
        digitalWrite(RELAIS_TRIGGER, HIGH);
        state = State::MEASURE;
    }
}

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
    //Serial.write("gemessene Distanz: ");
    //Serial.print(waterDistance);
    //Serial.write(", Wasserfüllstand: ");
    //Serial.print(percentage);
    //Serial.println('\n');
    return percentage >= MIN_WATER;
}
