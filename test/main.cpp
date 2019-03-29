//
// Created by cpasjuste on 29/03/19.
//

#include "main.h"
#include "p_search.h"

using namespace pscrap;

#define API_KEY "API_KEY"

void print_search(const Search &search) {
    printf("\nSearch results: pages: %i, total_results: %i, total_pages: %i\n",
           search.page, search.total_results, search.total_pages);
    for (auto &movie : search.movies) {
        printf("\n===========================\n");
        printf("%s\n", movie.title.c_str());
        printf("%s\n", movie.overview.c_str());
        printf("===========================\n");
    }
}

int main() {

    Search search(API_KEY, "Jack Reacher");

    int res = search.get();
    if (res == 0 && search.found) {
        print_search(search);
#if 0
        search.save("search.bin");
        Search loadedSearch;
        loadedSearch.load("search.bin");
        print_search(loadedSearch);
#endif
    }

    return 0;
}
