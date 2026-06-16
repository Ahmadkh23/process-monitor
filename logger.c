#include <stdio.h>
#include <time.h>
#include "monitor.h"

void log_action(const char *message){
    FILE *log_file = fopen("monitor.log" , "a");
    if (log_file == NULL) return ;

    time_t now ;
    time(&now);

    char *time_str = ctime(&now);
    time_str[24] = '\0' ;

    fprintf(log_file , "[%s] %s \n" , time_str , message);

    fclose(log_file);
}