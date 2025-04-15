#include <scheduler.h>
#include <software_timer.h>

#define LED_PIN 48

// Timer IDs
#define TIMER_TRAFFIC 0
#define TIMER_CLOCK 1

// Traffic Light States
#define TRAFFIC_INIT     0
#define RED_GREEN        1
#define RED_AMBER        2
#define GREEN_RED        3
#define AMBER_RED        4

// Clock States
#define CLOCK_INIT       0
#define CLOCK_RUN        1

int led_status = 0;
int trafficState = TRAFFIC_INIT;
int clockState = CLOCK_INIT;
int LED = 0;

void blinky() {
  if (led_status == 0) {
    digitalWrite(D3, HIGH); digitalWrite(D4, HIGH);
  } else {
    digitalWrite(D3, LOW);  digitalWrite(D4, HIGH);
  }
  led_status = 1 - led_status;
}

void trafficLight() {
  switch (trafficState) {
    case TRAFFIC_INIT:
      if (isTimerExpired(TIMER_TRAFFIC)) {
        setTimer(TIMER_TRAFFIC, 300);
        trafficState = RED_AMBER;
      }
      break;

    case RED_GREEN:
      if (isTimerExpired(TIMER_TRAFFIC)) {
        setTimer(TIMER_TRAFFIC, 300);
        trafficState = RED_AMBER;
      }
      digitalWrite(D3, HIGH); digitalWrite(D4, HIGH);
      digitalWrite(D5, HIGH); digitalWrite(D6, LOW);
      break;

    case RED_AMBER:
      if (isTimerExpired(TIMER_TRAFFIC)) {
        setTimer(TIMER_TRAFFIC, 200);
        trafficState = GREEN_RED;
      }
      digitalWrite(D3, HIGH); digitalWrite(D4, HIGH);
      digitalWrite(D5, LOW);  digitalWrite(D6, HIGH);
      break;

    case GREEN_RED:
      if (isTimerExpired(TIMER_TRAFFIC)) {
        setTimer(TIMER_TRAFFIC, 300);
        trafficState = AMBER_RED;
      }
      digitalWrite(D3, HIGH); digitalWrite(D4, LOW);
      digitalWrite(D5, HIGH); digitalWrite(D6, HIGH);
      break;

    case AMBER_RED:
      if (isTimerExpired(TIMER_TRAFFIC)) {
        setTimer(TIMER_TRAFFIC, 200);
        trafficState = RED_GREEN;
      }
      digitalWrite(D3, LOW);  digitalWrite(D4, HIGH);
      digitalWrite(D5, HIGH); digitalWrite(D6, HIGH);
      break;
  }
}

void displayOnClock(int LED) {
  clearClock();

  if (LED < 3) {
    switch (LED) {
      case 0: digitalWrite(D3, HIGH); digitalWrite(D4, HIGH); break;
      case 1: digitalWrite(D3, LOW);  digitalWrite(D4, HIGH); break;
      case 2: digitalWrite(D3, HIGH); digitalWrite(D4, LOW); break;
    }
  } else if (LED < 6) {
    switch (LED) {
      case 3: digitalWrite(D5, HIGH); digitalWrite(D6, HIGH); break;
      case 4: digitalWrite(D5, LOW);  digitalWrite(D6, HIGH); break;
      case 5: digitalWrite(D5, HIGH); digitalWrite(D6, LOW); break;
    }
  } else if (LED < 9) {
    switch (LED) {
      case 6: digitalWrite(D7, HIGH); digitalWrite(D8, HIGH); break;
      case 7: digitalWrite(D7, LOW);  digitalWrite(D8, HIGH); break;
      case 8: digitalWrite(D7, HIGH); digitalWrite(D8, LOW); break;
    }
  } else if (LED <= 11) {
    switch (LED) {
      case 9:  digitalWrite(D9, HIGH); digitalWrite(D10, HIGH); break;
      case 10: digitalWrite(D9, LOW);  digitalWrite(D10, HIGH); break;
      case 11: digitalWrite(D9, HIGH); digitalWrite(D10, LOW); break;
    }
  }
}

void clearClock() {
  digitalWrite(D3, LOW);  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);  digitalWrite(D8, LOW);
  digitalWrite(D9, LOW);  digitalWrite(D10, LOW);
}

void DigitalClock() {
  switch (clockState) {
    case CLOCK_INIT:
      if (isTimerExpired(TIMER_CLOCK)) {
        setTimer(TIMER_CLOCK, 500);
        clockState = CLOCK_RUN;
      }
      break;

    case CLOCK_RUN:
      if (isTimerExpired(TIMER_CLOCK)) {
        setTimer(TIMER_CLOCK, 500);
        LED = (LED + 1) % 12;
      }
      displayOnClock(LED);
      break;
  }
}

void TIMER_ISR(void *pvParameters) {
  while (1) {
    SCH_Update();
    vTaskDelay(10);
  }
}

void setup() {
  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);

  pinMode(D3, OUTPUT);  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);  pinMode(D10, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  SCH_Init();
//  setTimer(TIMER_TRAFFIC, 1);
  setTimer(TIMER_CLOCK, 1);
  SCH_Add_Task(timerRun, 0, 1);
 // SCH_Add_Task(trafficLight, 0, 1);
  SCH_Add_Task(DigitalClock, 0, 1);
}

void loop() {
  SCH_Dispatch_Tasks();
}
