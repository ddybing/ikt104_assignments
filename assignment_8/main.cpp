#include "mbed.h"

typedef struct {
    int minuteCounter;
    int secondCounter;
    Mutex mutex;
} Counters;


Thread thread_1;
Thread thread_2;
Thread thread_3;


void increaseMinuteCounter(Counters c){
    while (true) {
        c.mutex.lock();
        c.minuteCounter++;

        ThisThread::sleep_for(1000);
        c.mutex.unlock();
    }
}

void increaseSecoundCounter(Counters c){
      while (true) {
        c.mutex.lock();
        c.secondCounter++;

        ThisThread::sleep_for(100);
        c.mutex.unlock();
    }
}

void printClockValue(Counters c){
    while (true) {
        c.mutex.lock();
        printf("Clock: %i:%i", c.minuteCounter, c.secondCounter);

        ThisThread::sleep_for(100);
        c.mutex.unlock();
    }
}

int main()
{
    Counters c; 

    thread_1.start(increaseMinuteCounter(c));
    thread_2.start((increaseSecoundCounter(c)));
}

