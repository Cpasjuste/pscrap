//
// Created by cpasjuste on 29/03/19.
//

#include <curl/curl.h>
#include "p_movie.h"
#include "p_curl.h"

using namespace pscrap;

#define BASE_URL "https://image.tmdb.org/t/p/w"

int Movie::getPoster(const std::string &dstPath, int width) {

    if (poster_path.empty()) {
        printf("Movie::getPoster: error: poster_path is empty\n");
        return -1;
    }

    std::string url = BASE_URL + std::to_string(width) + "/" + poster_path;
    printf("Movie::getPoster: %s\n", url.c_str());

    int res = Curl::getData(url, dstPath);
    if (res != 0) {
        printf("Movie::getPoster: error: curl failed: %s\n",
               curl_easy_strerror((CURLcode) res));
        return res;
    }

    return 0;
}

int Movie::getBackdrop(const std::string &dstPath, int width) {

    if (backdrop_path.empty()) {
        printf("Movie::getBackdrop: error: poster_path is empty\n");
        return -1;
    }

    std::string url = BASE_URL + std::to_string(width) + "/" + backdrop_path;
    printf("Movie::getBackdrop: %s\n", url.c_str());

    int res = Curl::getData(url, dstPath);
    if (res != 0) {
        printf("Movie::getBackdrop: error: curl failed: %s\n",
               curl_easy_strerror((CURLcode) res));
        return res;
    }

    return 0;
}
