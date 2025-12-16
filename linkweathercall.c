#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
    CURL *curl;
	CURLcode res;

  /* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
	/* First set the URL that that program will handle */
	curl_easy_setopt(curl, CURLOPT_URL, "api.openweathermap.org/data/2.5/forecast?lat=-34.683331&lon=138.683334&appid=0c09cbd7b4cee723eeb324c9f47e9d25&units=metric");

	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));

	/* always cleanup */
	curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

//gcc linkcall.c -o linkcall -lcurl
//./linkcall > output.txt
/* monday 7:30am
{"dt":1765746000,"main":{"temp":12.84,"feels_like":11.82,"temp_min":12.84,"temp_max":12.84,
"pressure":1021,"sea_level":1021,"grnd_level":1007,"humidity":63,"temp_kf":0},"weather":
[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},
"wind":{"speed":2.51,"deg":138,"gust":6.41},"visibility":10000,"pop":0,"sys":{"pod":"d"},
"dt_txt":"2025-12-14 21:00:00"},	

Monday 7:30pm
{"dt":1765789200,"main":{"temp":18.17,"feels_like":17.11,"temp_min":18.17,"temp_max":18.17,
"pressure":1021,"sea_level":1021,"grnd_level":1007,"humidity":41,"temp_kf":0},"weather":
[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},
"wind":{"speed":5.3,"deg":142,"gust":8.02},"visibility":10000,"pop":0,"sys":{"pod":"d"},
"dt_txt":"2025-12-15 09:00:00"}
*/