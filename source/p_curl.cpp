//
// Created by cpasjuste on 29/03/19.
//

#include <curl/curl.h>
#include "p_curl.h"

using namespace pscrap;

static size_t write_string_cb(void *buf, size_t len, size_t count, void *stream) {
    ((std::string *) stream)->append((char *) buf, 0, len * count);
    return len * count;
}

static size_t write_data_cb(void *buf, size_t len, size_t count, void *stream) {
    size_t written = fwrite(buf, len, count, (FILE *) stream);
    return written;
}

std::string Curl::getString(const std::string &url) {

    std::string data;
    int res = 0;

    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Curl::getString: error: curl_easy_init failed\n");
        return data;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string_cb);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != 0) {
        printf("Curl::getString: error: curl_easy_perform failed: %s\n",
               curl_easy_strerror((CURLcode) res));
        return "";
    }

    return data;
}

int Curl::getData(const std::string &url, const std::string &dstPath) {

    FILE *data;
    int res = 0;

    data = fopen(dstPath.c_str(), "wb");
    if (!data) {
        printf("Curl::getData: error: fopen failed\n");
        return -1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Curl::getData: error: curl_easy_init failed\n");
        fclose(data);
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_cb);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(data);

    if (res != 0) {
        printf("Curl::getData: error: curl_easy_perform failed: %s\n",
               curl_easy_strerror((CURLcode) res));
        remove(dstPath.c_str());
        return res;
    }

    return 0;
}
