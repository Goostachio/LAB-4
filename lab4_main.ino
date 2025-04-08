#include <scheduler.h>
#include <software_timer.h>
#define LED_PIN 48
#define D3 6
#define D4 7
#define D5 8
#define D6 9

#define INIT 0
#define RED_GREEN  1
#define RED_AMBER  2
#define GREEN_RED  3
#define AMBER_RED  4

int led_status = 0;

void blinky(){
	if(led_status == 0){
      digitalWrite(6, HIGH);digitalWrite(7, HIGH);
    }else{
      digitalWrite(6, LOW);digitalWrite(7, HIGH);
    }
    led_status = 1 - led_status;
}

int state = RED_GREEN;

void trafficLight(){
  switch (state) {
    case RED_GREEN:
      if (isTimerExpired(0) == 1) {
        setTimer(0, 300);
        state = RED_AMBER;
      }

      digitalWrite(6, HIGH);digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);digitalWrite(9, LOW);
      break;

    case RED_AMBER:
      if (isTimerExpired(0) == 1) {
        setTimer(0, 200);
        state = GREEN_RED;
      }
      digitalWrite(6, HIGH);digitalWrite(7, HIGH);
      digitalWrite(8, LOW);digitalWrite(9, HIGH);

      break;

    case GREEN_RED:
      if (isTimerExpired(0) == 1) {
        setTimer(0, 300);
        state = AMBER_RED;
      }
      digitalWrite(6, HIGH);digitalWrite(7, LOW);
      digitalWrite(8, HIGH);digitalWrite(9, HIGH);
      
      break;

    case AMBER_RED:
      if (isTimerExpired(0) != 1) {
        setTimer(0, 200);
        state = RED_GREEN;
        }
      digitalWrite(6, LOW);digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);digitalWrite(9, HIGH);
      
      break;
  }
}

void TIMER_ISR(void *pvParameters) {
  while(1) {
    SCH_Update();
    vTaskDelay(10);
  }
}

void setup() {
  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);
  
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  SCH_Init();
  setTimer(0, 300);
  SCH_Add_Task(timerRun, 0, 1);
  SCH_Add_Task(trafficLight, 0, 1);
}

void loop() {
  SCH_Dispatch_Tasks();
}
