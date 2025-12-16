/*
deployment link: https://script.google.com/macros/s/AKfycbyJEpuRUwkbcVy-DxwJlOmzs3S1xoh14p2dE5bIZeSGBEGBkyOXRFYWNMQon6NuMVYxlg/exec
*/
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

void    parse_event_str(char *calendar_data)
{
    char    *search;
    char    event[200];
    char    *substring;
    int     event_len = 0;
    search = strstr(calendar_data, "title");
    if (search)
    {
        substring = search + 8;
        event_len = strstr(substring, "start") - substring - 4;
        strncpy(event, substring, event_len);
        printf("\n\nfinal string: %s\n\n", event);
    }
    return ;
}


// 1. IMPROVED WRITE FUNCTION: Safer and handles larger data
size_t write_data(void *ptr, size_t size, size_t nmemb, char *data) {
    // nmemb * size is the number of bytes being sent by curl
    // We append it to our buffer. 
    // WARNING: In a real project, use a dynamic buffer or check bounds!
    strncat(data, (char *)ptr, size * nmemb); 
    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    char calendar_data[8192] = ""; // Expanded to 8KB for more events

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        // Your Google Script URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://script.google.com/macros/s/AKfycbyJEpuRUwkbcVy-DxwJlOmzs3S1xoh14p2dE5bIZeSGBEGBkyOXRFYWNMQon6NuMVYxlg/exec");

        // 2. THE CRITICAL FIX: Tell curl to follow the "302 Moved" redirect
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // 3. SSL FIX: Sometimes needed on local machines to avoid certificate errors
        // Only use this for debugging; for production, you should use certificates.
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Setup the data capture
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, calendar_data);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // SUCCESS! Now your variable 'calendar_data' contains the raw JSON.
        //    printf("--- Calendar JSON Data ---\n%s\n", calendar_data);
            parse_event_str(calendar_data);
        }
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
//    parse_calendar_data(calendar_data);
    return 0;
}
/*gcc linkcalendarcall.c -o linkcalendarcall -lcurl*/