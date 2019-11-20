//
// Created by cpasjuste on 29/03/19.
//

#ifndef PSCRAP_P_SEARCH_H
#define PSCRAP_P_SEARCH_H

#include <vector>
#include <json-c/json.h>
#include "p_movie.h"

namespace pscrap {

    class Search {

    public:

        // TODO: handle tv shows
        enum class Type {
            Movie,
            TvShow
        };

        /// empty search, to be loaded from file with "load"
        Search() = default;

        /// search for a movie, using curl and json
        explicit Search(const std::string &api_key, const std::string &name,
                        const std::string &language = "en-US", Type type = Type::Movie);

        /// proceed the search
        /// return 0 on success, curl error code on error
        int get(int *http_code = nullptr);

        /// load a saved search from a file
        int load(const std::string &srcPath);

        /// save a search to a file
        int save(const std::string &dstPath);

        int page = 0;
        int total_results = 0;
        int total_pages = 0;
        std::vector<Movie> movies;

    private:

        void parseMovieRoot(const std::string &jsonData);

        void parseMovie(json_object *obj);

        static bool sortByTitleDistance(const Movie &movie1, const Movie &movie2, const std::string &title);

        std::string url;
        std::string data;
        std::string search;
        std::string search_escaped;
        std::string api_key;
        std::string language;
    };
}

#endif //PSCRAP_P_SEARCH_H
