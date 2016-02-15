#include "timer.h"

void StartTimer(){
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    interval_time = start_time;
}
void UpdateTimer(){
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &interval_time);
}
long GetTime(){
	struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = end_time.tv_nsec - start_time.tv_nsec;
    return diffInNanos/1000;
}
long GetDeltaTime(){
	struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = end_time.tv_nsec - interval_time.tv_nsec;
    return diffInNanos/1000;
}
