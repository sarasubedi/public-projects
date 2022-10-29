/* temperature_data.c


   B. Bird - 11/09/2021
   
   Sara Subedi (V00986656) - 11/30/2021 
   
   This program reads in single or whole temperature inputs through text files, count those occurances, 
   can find station extremes and print daily averages (in chronological order).
   
*/

#include <stdio.h>
#include "temperature_data.h"

/* Do not copy the typedef statements for the structure types into this file
   (they should only be in the .h file)
*/


/* read_observation(input_file, obs)
   Given a file object input_file and a pointer obs to an Observation 
   structure, try to read a single observation from the file into the structure
   pointed to by obs. If the complete observation is read successfully, return 1.
   Otherwise, return 0.

   Parameters: input_file (FILE pointer), obs (pointer to Observation object)
   Return value (integer):
     - If the observation was read successfully, return 1
     - Otherwise, return 0
*/
int read_observation(FILE* input_file, Observation* obs){
    /* Your code here */
    int values_read = fscanf(input_file, "%d %d %d %d %d %d %f", 
                             &(obs->obs_date.year), &(obs->obs_date.month), 
                             &(obs->obs_date.day), &(obs->hour), &(obs->minute), 
                             &(obs->station_id), &(obs->temperature));
    
    if(values_read != 7){
        return 0;
    } else {
        return 1;
    }
    fclose(input_file);
}


/* count_observations(filename)
   Given a filename, count and return the number of temperature observations in
   the file. If the file cannot be opened, return -1.

   Parameters: filename (string)
   Return value (integer):
     - If the file cannot be opened for reading, return -1
     - Otherwise, return the number of temperature observations in the file.
*/
int count_observations(char filename[]){
    /* Your code here */
    
    int obs = 0; // return value
    int d; // int holder
    float f; // float holder
    
    FILE* input_file = fopen(filename,"r");
    
    // unable to read input file
    if(input_file == NULL){
        return -1;
    } // if
    
    while(fscanf(input_file, "%d %d %d %d %d %d %f", &d, &d, &d, &d, &d, &d, &f) == 7){
        obs++;
    } // while
    
    fclose(input_file);
    return obs;
    
} // count_observations

/* load_all_observations(filename, array_size, observation_array)
   Given a filename and an array of Observation objects, read as many 
   observations from the file as possible (up to the size of the array)
   and store each observation (in order) into the array.

   If the file cannot be opened, return -1. Otherwise, return the number of 
   observations read.

   If the file contains fewer observations then there are elements in the array,
   stop reading after the last observation in the file. Otherwise, stop reading
   once array_size observations are read.

   Parameters: filename (string), array_size (integer), observation_array (array of Observation)
   Return value (integer):
     - If the file could not be opened, return -1
     - Otherwise, return the total number of observations read and stored into 
       the array.
*/
int load_all_observations(char filename[], int array_size, Observation observation_array[array_size]){
    /* Your code here */
    
    int obs = 0; // return value
    
    FILE* input_file = fopen(filename,"r");
    
    if(input_file == NULL){
        return -1;
    } // if
    
    // read in observations and but them into an array
    for(int i = 0; i < array_size; i++){
        int values_read = fscanf(input_file, "%d %d %d %d %d %d %f", 
                                 &(observation_array[i].obs_date.year), 
                                 &(observation_array[i].obs_date.month), 
                                 &(observation_array[i].obs_date.day), 
                                 &(observation_array[i].hour), 
                                 &(observation_array[i].minute), 
                                 &(observation_array[i].station_id),
                                 &(observation_array[i].temperature));
            if(values_read != 7){
                i--;
                continue;
            } // if
        obs++;
    } // for
    
    fclose(input_file);
    return obs;
   
} // load_all_observations

// return 1 if b is older than a, 0 if a is older than b 
// and -1 if equal

int compare_obs (Observation a, Observation b) {
    
    Date da = a.obs_date;
    Date db = b.obs_date;
    
    if(da.year > db.year) {
        return 1;
    } else if(da.year < db.year) {
        return 0;
    } else if (da.year == db.year) {
        
        if(da.month > db.month){
            return 1;
        } else if(da.month < db.month){
            return 0;
        } else if(da.month == db.month){
            
            if(da.day > db.day){
                return 1;
            } else if(da.day < db.day){
                return 0;
            } else if (da.day == db.day){
                
                if(a.hour > b.hour){
                    return 1;
                } else if(a.hour < b.hour){
                    return 0;
                } else if(a.hour == b.hour){
                    
                    if(a.minute > b.minute){
                        return 1;
                    } else if(a.minute < b.minute){
                        return 0;
                    } else if(a.minute == b.minute){
                        return -1;
                    } // else if
                    
                } // else if
                
            } // else if
            
        } // else if
        
    } // else if 
    
    return -999;
    
} // compare_obs


/* print_station_extremes(num_observations, obs_array)
   Given an array of Observation objects, compute and print the
    _extreme observations_ (lowest temperature observed and highest
   temperature observed) for each station that has at least one
   observation in the dataset.

   The output will contain one line for each station with at least one 
   observation in the dataset, using a format equivalent to the following:
      Station 1: Minimum = -1.87 degrees (2021-11-21 06:10), Maximum = 10.6 degrees (2021-01-11 01:16)

   You should use the following printf format string to achieve the correct 
   output format.
      "Station %d: Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n"
 
   The output must be ordered by station number (with lower numbered station 
   numbers appearing first). No more than one line of output should be 
   generated for a particular station number.

   In cases where multiple observations achieve the extreme value (e.g. if the
   minimum temperature at a particular station is -1.87, but there are several 
   observations with this temperature), print the date/time of the 
   chronologically earliest observation with that extreme temperature.

   You may assume that all observations contain a station number between 
   1 and 250 (inclusive).

   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of station extremes is output to the user.
*/
void print_station_extremes(int num_observations, Observation obs_array[num_observations]){
    /* Your code here */
    
    int idx_max = 0;
    int idx_min = 0;
    float station_min[251];
    float station_max[251];
    
    for(int i = 0; i <= 250; i++){
        station_max[i] = -9999;
        station_min[i] = 9999;
    } // for
    
    for(int i = 1; i <= 250; i++){
        int occur = 0;
        for(int j = 0; j < num_observations; j++){
            
            if(obs_array[j].station_id == i) {
                occur++;
                if(obs_array[j].temperature > station_max[i]){
                    station_max[i] = obs_array[j].temperature;
                    idx_max = j;
                } else if(obs_array[j].temperature == station_max[i]) {
                    if(compare_obs(obs_array[j], obs_array[idx_max]) == 0){
                        station_max[i] = obs_array[j].temperature;
                        idx_max = j;
                    } // if
                } // else if
                if (obs_array[j].temperature < station_min[i]){
                    station_min[i] = obs_array[j].temperature;
                    idx_min = j;
                } else if(obs_array[j].temperature == station_min[i]) {
                    if(compare_obs(obs_array[j], obs_array[idx_min]) == 0){
                        station_min[i] = obs_array[j].temperature;
                        idx_min = j;
                    } // if
                } // else if
            } // if
        } // inner for
        
        if(occur > 0){
        printf("Station %d: Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), ", 
                   (i), station_min[i], 
                   obs_array[idx_min].obs_date.year, obs_array[idx_min].obs_date.month, 
                   obs_array[idx_min].obs_date.day, obs_array[idx_min].hour, obs_array[idx_min].minute);
            printf("Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n", 
                   station_max[i], obs_array[idx_max].obs_date.year, 
                   obs_array[idx_max].obs_date.month, obs_array[idx_max].obs_date.day, 
                   obs_array[idx_max].hour, obs_array[idx_max].minute);
        } // if
    }// outer for
} // print_station_extremes

/* print_daily_averages(num_observations, obs_array)
   Given an array of observation objects, compute and print the average 
   temperature for each day which has at least one observation in the 
   dataset.

   The output must contain only dates which actually appear in at least 
   one observation object in the array. The dates must be in ascending 
   chronological order (so an earlier date must be printed before a later 
   one) and each date may only appear once.

   Each line of output will have the form "year month day average", for 
   example "2021 11 20 10.6" (which would be interpreted to mean that 
   the average temperature on Nov. 21, 2021 was 10.6 degrees).

   Your code may assume that all dates are from years between 2015 and 2021 
   (inclusive) and that the month/day entries are all valid (that is, month
   will always be between 1 and 12 inclusive and day will always be between
   1 and 31 inclusive).
   
   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of the average daily temperature is
                output to the user.
*/
void print_daily_averages(int num_observations, Observation obs_array[num_observations]){
    /* Your code here */
    float y2021[13][32] = { {0} };
    float y2020[13][32] = { {0} };
    float y2019[13][32] = { {0} };
    float y2018[13][32] = { {0} };
    float y2017[13][32] = { {0} };
    float y2016[13][32] = { {0} };
    float y2015[13][32] = { {0} };
    int occurances[13][32] = { {0} };

    for(int i = 2015; i <= 2021; i++){
        
        for(int x = 0; x <= 12; x++){
            for(int y = 0; y <= 31; y++){
                y2021[x][y] = 0;
                y2020[x][y] = 0;
                y2019[x][y] = 0;
                y2018[x][y] = 0;
                y2017[x][y] = 0;
                y2016[x][y] = 0;
                y2015[x][y] = 0;
                occurances[x][y] = 0;
            } // inner for
        } // outer for
        
        for(int j = 0; j < num_observations; j++){
            
            if(obs_array[j].obs_date.year == i) {
                
                if(i == 2015){
                    y2015[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2016){
                    y2016[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2017){
                    y2017[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2018){
                    y2018[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2019){
                    y2019[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2020){
                    y2020[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } else if(i == 2021){
                    y2021[obs_array[j].obs_date.month][obs_array[j].obs_date.day] += obs_array[j].temperature;
                    occurances[obs_array[j].obs_date.month][obs_array[j].obs_date.day]++;
                } // else if
            } // if
        } // for
        
        for(int x = 0; x <= 12; x++){
            for(int y = 0; y <= 31; y++){
                if(i == 2015){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2015[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2016){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2016[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2017){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2017[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2018){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2018[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2019){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2019[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2020){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2020[x][y]/occurances[x][y]));
                    } // if
                    
                } else if(i == 2021){
                    if(occurances[x][y] != 0){
                        printf("%d %d %d %.1f\n", i, x, y, (y2021[x][y]/occurances[x][y]));
                    } // if
                    
                } // else if
            } // inner for
        } // outer for
    } // outermost for (year incrementation)
} // print_daily_averages