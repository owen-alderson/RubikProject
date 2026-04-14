#include "timer.h"

long start_time = 0;
bool timer_running = false;

void start_timer() {
    start_time = clock();
    timer_running = true;
}

void stop_timer() {
    timer_running = false;
}

void reset_timer() {
    start_time = 0;
    timer_running = false;
}
