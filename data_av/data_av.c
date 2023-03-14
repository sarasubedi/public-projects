//
//  data_av.c
//  
//
//  Created by Sara Subedi on 3/13/23.
//

#include "data_av.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct cityStat{
    char *city;
    float min;
    float max;
    int numEntries;
    double average;
} cityStat;

#define NUMCITIES 10

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

pthread_t threadIDs[NUMCITIES];
int threading = 0;

//GLOBAL VARS THAT NEED TO BE LOCKED
cityStat *data[10];
int idx = 0;

void *checked_malloc(size_t bytes) {
    void* p;
    p = malloc(bytes);
    if(!p) {
        fprintf(stderr, "malloc failed!");
        exit(0);
    }
    return p;
}

cityStat *init_cityStat(char *cityName){
    data[idx] = (cityStat *)checked_malloc(sizeof(cityStat));
    data[idx]->min = 9999999;
    data[idx]->max = -9999999;
    data[idx]->city = (char *)checked_malloc((strlen(cityName)+1)*sizeof(char));
    strncpy(data[idx]->city, cityName, strlen(cityName)+1);
    data[idx]->numEntries = 0;
    data[idx]->average = 0;
    return data[idx++];
}

void printStats(int i) {
    printf("------------------------------------\n"
            "%s city\n"
            "%s's lowest temperature is: %.1f degrees Celsius\n"
            "%s's highest temperature is: %.1f degrees Celsius\n"
            "The average temperature for %s is: %.5lf degrees Celsius\n"
            "Total values processed for %s are: %d\n"
            "------------------------------------\n\n",
            data[i]->city,data[i]->city,data[i]->min,data[i]->city,data[i]->max,
            data[i]->city,data[i]->average,data[i]->city,data[i]->numEntries);
}

void processFile(char *cityName) {
    FILE* f;
    char buff[20];
    int buffLen = 90;

    char fileName[30] = "data_files/";
    strncat(fileName, cityName, strlen(cityName));
    strncat(fileName, ".dat", 5);
    f = fopen(fileName,"r");
    if(!f) {
        printf("ERROR: %s not found.\n", fileName);
        return;
    }
    //LOCK idx
    //printf("lock [%s]\n",cityName);
    while(pthread_mutex_lock(&qlock));
    /*
    if(pthread_mutex_lock(&qlock)){
        printf("wait\n");
        pthread_cond_wait(&qready, &qlock);
        printf("going\n");
    }*/
    int cur = idx;
    cityStat* temp = init_cityStat(cityName);
    pthread_mutex_unlock(&qlock);
    //printf("unlock [%s]\n", cityName);
    pthread_cond_signal(&qready);
    //printf("signal[%s]\n", cityName);
    //UNLOCK idx

    while(fgets(buff, buffLen, f)) {
        float min;
        float max;
        int i = sscanf(buff, "%f %f", &max, &min);
        if(i == 1) {
            if(max > temp->max) temp->max = max;
            if(max < temp->min) temp->min = max;
            temp->numEntries += 1;
            temp->average += max;
        } else if(i == 2) {
            if(max > temp->max) temp->max = max;
            if(min < temp->min) temp->min = min;
            temp->numEntries += 2;
            temp->average += max+min;
        }
    }
    if(temp->numEntries) { // incase division by zero
        temp->average/=temp->numEntries;
    }
    fclose(f);
    printStats(cur);
    if(threading) pthread_exit(NULL);
}

void printConcl() {
    cityStat total;
    total.min = 99999;
    char *minPlace;
    total.max = -99999;
    char *maxPlace;
    total.numEntries = 0;
    for(int i = 0; i < idx; i++) {
        if(total.min > data[i]->min) {
            total.min = data[i]->min;
            minPlace = data[i]->city;
        }
        if(total.max < data[i]->max) {
            total.max = data[i]->max;
            maxPlace = data[i]->city;
        }
        total.numEntries += data[i]->numEntries;
    }
    printf("Total values evaluated is: %d\n", total.numEntries);
    if(!idx) return;
    printf("The lowest temperature overall is: %.1f reported in: %s\n", total.min, minPlace);
    printf("The highest temperature overall is: %.1f reported in: %s\n\n", total.max, maxPlace);
}

int main(int argc, char* argv[]) {
    char* cities[] = {"Charlottetown", "Edmonton", "Halifax", "Montreal",
                        "Ottawa", "Quebec", "Toronto", "Vancouver", "Victoria", "Winnipeg"};
    
    if(argc == 2 && !strncmp(argv[1],"-m", 3)) {
        //printf("MULTITHREADED OPT\n");
        threading = 1;
        for(int i = 0; i < NUMCITIES; i++) {
            //printf("start thread %d\n", i+1);
            pthread_create(&threadIDs[i], NULL, (void *)processFile, (void *)cities[i]);
        }
        for (int i=0; i < NUMCITIES; i++) {
            //printf("waiting for thread\n");
            pthread_join (threadIDs[i], NULL);
            //printf("end thread %d\n", i+1);
        }
        //printf("All threads are terminated...\n");
    } else for(int i = 0; i < NUMCITIES; i++) processFile(cities[i]);
    printConcl();
    printf("Elapsed time: %ld clocks\n", clock());
    
    for(int x = 0; x < idx; x++){
        free(data[x]->city);
        free(data[x]);
    }
}
