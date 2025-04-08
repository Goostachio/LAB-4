#include "software_timer.h"

int flag[MAX];
int counter[MAX];

void setTimer(int index, int _counter) {
    counter[index] = _counter;
    flag[index] = 0;
}

int isTimerExpired (int index) {
    if (flag[index] == 1){
        flag[index] = 0;
        return 1;
    }
    return 0;
}

void timerRun(){
    for(int i = 0; i < MAX; i++){
        if(counter[i] > 0){
            counter[i] --;
            if(counter[i] == 0) flag[i] = 1;
        }
    }
}
