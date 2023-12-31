#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

//return value of 'size_t' is byte, 'contents' holds data that recieve/arrives,'size' is size/byte taken by each element,'nmemb' is number of elements/member
size_t write_callback(void *contents, size_t size, size_t nmemb, FILE *file) {
    return fwrite(contents, size, nmemb, file);
}


int main(void)
{   
  FILE *file;
  // Stores the CURL handle used to manage the request and easy API session
  CURL *curl;

  // Stores the return value of the call to curl_easy_perform()
  CURLcode result;

  // Starts the session, return the curl handle we'll use to setup the request
  curl = curl_easy_init();

  // If curl_easy_init() fails the function returns an error, we exit with an
  // error message and status in this case
  if (curl == NULL)
  {
    fprintf(stderr, "HTTP request failed\n");
    return -1;
  }

  //  lat":24.8546842,"lon":67.0207055
  // curl_easy_setopt() is used to set the options for the request, we MUST set
  // the CURLOPT_URL, i.e. where the request will be to, and we setup a request
  // to google.com.

 curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=karachi&appid=38ae9bcfd18b0c93ce389640e87d7e59");

// Open a file for writing
    file = fopen("response.txt", "ab");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    // Set the write callback function to write to the file the data that is received/arrives from request 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);



  // curl_easy_perform() will perform the request and return a CURLcode result.
  // By default the function will output the response to standard output (i.e.
  // the terminal by default).  We could store the response in a string using
  // the approach below.
  result = curl_easy_perform(curl);

  // If the request is OK we'll get back CURLE_OK as a return value, so if we
  // don't get back CURLE_OK something has gone wrong and we'll exit with an
  // error message and status.
  if (result != CURLE_OK)
  {
    // The function curl_easy_strerror() will return an error message string
    // which we output, when it is supplied result as an argument (i.e. the
    // return value from the call to curl_easy_perform()).
    fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    return -1;
  }

  // We call curl_easy_cleanup() to complete the session.
  fprintf(file,"\n");
  fclose(file);
  curl_easy_cleanup(curl);

  return 0;
}
