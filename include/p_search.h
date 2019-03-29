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

        enum class Type {
            Movie,
            TvShow
        };

        explicit Search(const std::string &api_key, const std::string &name,
                        const std::string &language = "en-US", Type type = Type::Movie);

        /// return curl error code
        int get();

        /// return json string
        std::string getJson() const;

        int page;
        int total_results;
        int total_pages;
        std::vector<Movie> movies;

    private:

        void parseMovieRoot();

        void parseMovie(json_object *obj);

        std::string curl_url;
        std::string curl_data;
    };
}

#endif //PSCRAP_P_SEARCH_H
