#ifndef TIMER_H
#define TIMER_H

#include <time.h>

struct timespec start_time;
struct timespec interval_time;

void StartTimer();
void UpdateTimer();
long GetTime();
long GetDeltaTime();

#endif
