#ifndef TIMER_H
#define TIMER_H
#include <stdbool.h>
#include <time.h>

void start_timer();
void stop_timer();
void reset_timer();

extern long start_time;
extern bool timer_running;

#endif
