#include <scheduler.h>
#define LED_PIN 48
#define D3 6
#define D4 7
#define D5 8
#define D6 9

int led_status = 0;

void blinky(){
	if(led_status == 0){
      digitalWrite(D3, HIGH);digitalWrite(D4, HIGH);
    }else{
      digitalWrite(D3, LOW);digitalWrite(D4, HIGH);
    }
    led_status = 1 - led_status;
}

int state = 0;
int counter = 0;

void trafficLight(){
  switch (state) {
    case 0: // red_green
      digitalWrite(6, HIGH);digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);digitalWrite(9, LOW);
      counter++;
      if (counter >= 3) { state = 1; counter = 0; }
      break;

    case 1: // red_amber
      digitalWrite(6, HIGH);digitalWrite(7, HIGH);
      digitalWrite(8, LOW);digitalWrite(9, HIGH);
      counter++;
      if (counter >= 2) { state = 2; counter = 0;}
      break;

    case 2: // green_red
      digitalWrite(6, HIGH);digitalWrite(7, LOW);
      digitalWrite(8, HIGH);digitalWrite(9, HIGH);
      counter++;
      if (counter >= 3) { state = 3; counter = 0;}
      break;

    case 3: // amber_red
      digitalWrite(6, LOW);digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);digitalWrite(9, HIGH);
      counter++;
      if (counter >= 2) { state = 0; counter = 0;}
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
  pinMode(LED_PIN, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, LOW);

  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);

  SCH_Init();
  SCH_Add_Task(trafficLight, 0, 100);

  
}

void loop() {
  SCH_Dispatch_Tasks();
}