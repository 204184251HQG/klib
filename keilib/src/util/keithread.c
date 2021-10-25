#include "util/apipthread.h"
#include "log/keilog.h"
#include <stdio.h>
#include <stdlib.h>

int num_online_threads(void){
    FILE *status_f = fopen("/proc/self/status", "rb");
    if(!status_f){
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, status_f)>0){
        //KLOG_I("getline %s\n", line);
        const char *fmt_match = "Threads:\t";
        if(strncmp(line, fmt_match, strlen(fmt_match)) == 0){
            const char *tnum_str = line + strlen(fmt_match);
            int tnum = atoi(tnum_str);
            if(tnum){
                //KLOG_I("tnum %d\n", tnum);
                return tnum;
            }
        }
    }
    return 1;
}

thread_id_t create_thread(void *(*func)(void *), void *arg)
{
    thread_id_t tid;
    if (pthread_create(&tid, NULL, func, arg) != 0) {
        perror("create_thread:pthread_create");
        exit(EXIT_FAILURE);
    }
    return tid;
}