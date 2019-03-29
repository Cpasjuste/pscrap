//
// Created by cpasjuste on 29/03/19.
//

#include "main.h"
#include "p_search.h"

using namespace pscrap;

int main() {

    Search search("API_KEY", "Jack+Reacher");

    int res = search.get();
    if (res == 0) {
        printf("=====\nSearch results\n=====\n");
        printf("pages: %i, total_results: %i, total_pages: %i\n",
               search.page, search.total_results, search.total_pages);
        for (auto &movie : search.movies) {
            printf("=====\nMovie\n=====\n");
            printf("Title: %s\n", movie.title.c_str());
            printf("Overview: %s\n", movie.overview.c_str());
        }
        // get poster
        search.movies.at(0).getPoster("poster.jpg");
        search.movies.at(0).getBackdrop("backdrop.jpg");
    } else {
        printf("main: error: search failed\n");
    }

    return 0;
}
