#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 5000

/* // create a struct to contain all the info // */

//typedef struct weather_info {
//    float temp_max = 0;
//    float temp_min = 1000;
//    char des_tmp[20];
//    int des_len = 0;
//} t_weather;

void parse_weather_data(char *weather_info)
{
    int iteration = 1;
    char *search;
    char *pEnd;
    int description_bool = 1;//boolean to check we've already found the first description of the weather
    float tmp;//tmp variable to store the current temp
    float temp_max = 0; //tmp variable to store the current temp
    float temp_min = 1000;//tmp variable to store the current temp
    char des_tmp[20] = {0};
    int des_len = 0;//to determine how long the description is
    char str_temp_min[20];
    char *substring;
    //strncpy(weather_info, weather_info, line_len);
    search = strstr(weather_info, "temp_min");
//    printf("search is: %s\n\n%d characters long", search, line_len);
    while (search) {
    //    printf("line: %s\n", weather_info);
        // Use strstr to check for the search string in the current line
        if (search) {
            substring = search + 10;
            strncpy(str_temp_min, substring, 4);
            str_temp_min[4] = '\0';
            tmp = strtof(str_temp_min, &pEnd);
            if (tmp < temp_min)
                temp_min = tmp;
            if (tmp > temp_max)
                temp_max = tmp;
        }
        if (description_bool)
        {
            substring = search + 164;
            des_len = strchr(substring, '"') - substring + 1; // determine length of description text
            strlcpy(des_tmp, substring, des_len); // copy weather description into des_tmp
            des_tmp[des_len] = '\0';
            description_bool = 0;
        }
    //    printf("substring points to = '%.10s' new search (%d) points to:\n%.10s\n\n", substring, iteration, search);
        search = strstr(substring, "temp_min"); //iterate the search by looking for the next temp_min occurence after description
        iteration++;
    }
    printf("des_tmp: %s, min: %f max: %f\n", des_tmp, temp_min, temp_max);
    return ;
}

// SAFE WRITE FUNCTION
size_t write_data(void *ptr, size_t size, size_t nmemb, char *data) {
    size_t incoming_len = size * nmemb;
    size_t current_len = strlen(data);
    
    // 1. Stop if we've reached our 8-line goal (e.g., ~2500 chars)
    if (current_len >= 2500) return 0;

    // 2. Prevent Buffer Overflow: calculate remaining space in the array
    size_t capacity_left = (BUFFER_SIZE - 1) - current_len;
    size_t amount_to_copy = (incoming_len < capacity_left) ? incoming_len : capacity_left;

    strncat(data, (char *)ptr, amount_to_copy);
    return incoming_len; // Tell curl we "handled" it all to avoid errors
}

int main(void) {
    CURL *curl;
    CURLcode res;
    // This is your 'bucket'
    char weather_info[BUFFER_SIZE] = ""; 

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "api.openweathermap.org/data/2.5/forecast?lat=-34.683331&lon=138.683334&appid=0c09cbd7b4cee723eeb324c9f47e9d25&units=metric");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Link the bucket (weather_info) to the write function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, weather_info);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK || res == CURLE_WRITE_ERROR) {
            // Now we call the parser on the bucket we just filled
            printf("weather_info: %s\n\n\n", weather_info);
            parse_weather_data(weather_info);
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}

// figure out how to read each line (every 490 characters) and search for tmp_min/temp_max etc

//gcc readweatheroutput.c -o readweatheroutput -lcurl
//./readweatheroutput.c