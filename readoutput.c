#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main()
{
    FILE *file_opened;
    char line[BUFFER_SIZE];
//    char *search_strings[] = {"temp_min", "temp_max", "description"};
    char *filename;
    int i = 0;
    int line_num = 0;
    int found_count = 0;
    char *search;
    float tmp;
    int des_len = 0;
    char *pEnd;
    float temp_max = 0;
    float temp_min = 1000;
    char str_temp_min[20];
    char des_tmp[20];
    char *substring;
    filename = "output.txt";
    file_opened = fopen(filename, "r");
    if (file_opened == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    while (fgets(line, BUFFER_SIZE, file_opened) != NULL && line_num < 8) {
        line_num++;
        // Use strstr to check for the search string in the current line
        search = strstr(line, "temp_min");
        if (search) {
            substring = search + 10;
        //    printf("\n%d: %.4s", found_count, substring);//delete
            // If the line doesn't end with a newline character, add one for formatting
            if (line[strlen(line) - 1] != '\n') {
                printf("\n");
            }
            found_count++;//delete
            strncpy(str_temp_min, substring, 4);
            substring[4] = '\0';
            tmp = strtof(str_temp_min, &pEnd);
            if (tmp < temp_min)
                temp_min = tmp;
            if (tmp > temp_max)
                temp_max = tmp;
        }
    //    search = strstr(line, "description");
        if ((des_len == 0) && (search = strstr(line, "description")))
        {
            substring = search + 14;
            if (line[strlen(line) - 1] != '\n') {
                printf("\n");
            }
        //    printf("description found: %.1s\n", substring);
            des_len = strchr(substring, '"') - substring;
            strncpy(des_tmp, substring, des_len);
            printf("des_tmp: %s\n", des_tmp);
        }
    }
    printf("temp_min: %f temp_max: %f\n", temp_min, temp_max);
    fclose(file_opened);
    printf("\n\nfound: %d\n", found_count);
    return (0);

}

//gcc readoutput.c -o readoutput -lcurl
//./readoutput.c