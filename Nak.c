#include <time.h>


void getLocalTime(struct tm *localTime) {
    time_t currentTime;
    time(&currentTime);
    *localTime = *localtime(&currentTime);
}