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

std::string Curl::getString(const std::string &url, int *http_code) {

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
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

    res = curl_easy_perform(curl);
    if (http_code) {
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &(*http_code));
    }
    curl_easy_cleanup(curl);

    if (res != 0) {
        printf("Curl::getString: error: curl_easy_perform failed: %s, http_code: %i\n",
               curl_easy_strerror((CURLcode) res), http_code ? *http_code : 0);
        return "";
    }

    return data;
}

int Curl::getData(const std::string &url, const std::string &dstPath, int *http_code) {

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
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

    res = curl_easy_perform(curl);
    fclose(data);
    if (http_code) {
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &(*http_code));
    }
    curl_easy_cleanup(curl);

    if (res != 0) {
        printf("Curl::getData: error: curl_easy_perform failed: %s, http_code: %i\n",
               curl_easy_strerror((CURLcode) res), http_code ? *http_code : 0);
        remove(dstPath.c_str());
        return res;
    }

    return 0;
}
