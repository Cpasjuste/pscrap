//
// Created by cpasjuste on 29/03/19.
//

#include <cstring>
#include "p_search.h"
#include "p_curl.h"

using namespace pscrap;

Search::Search(const std::string &api_key, const std::string &name, const std::string &language, Type type) {

    curl_url = "https://api.themoviedb.org/3/search/movie?api_key="
               + api_key + "&language=" + language + "&query=" + name
               + "&page=1&include_adult=false";
}

int Search::get() {

    curl_data = Curl::getString(curl_url);
    if (curl_data.empty()) {
        return -1;
    }

    parseMovieRoot();

    return 0;
}

std::string Search::getJson() const {
    return curl_data;
}

void Search::parseMovieRoot() {

    json_object *root = json_tokener_parse(curl_data.c_str());
    json_object_object_foreach(root, key, obj) {
        enum json_type type = json_object_get_type(obj);
        switch (type) {
            case json_type_int:
                if (strcmp(key, "page") == 0) {
                    page = json_object_get_int(obj);
                } else if (strcmp(key, "total_results") == 0) {
                    total_results = json_object_get_int(obj);
                } else if (strcmp(key, "total_pages") == 0) {
                    total_pages = json_object_get_int(obj);
                }
                break;
            case json_type_array:
                if (strcmp(key, "results") == 0) {
                    parseMovie(obj);
                }
                break;

            default:
                break;
        }
    }
}

void Search::parseMovie(json_object *root) {

    int count = json_object_array_length(root);

    for (int i = 0; i < count; i++) {

        json_object *array = json_object_array_get_idx(root, i);
        enum json_type type = json_object_get_type(array);

        if (type == json_type_object) {
            Movie movie{};
            json_object_object_foreach(array, key, obj) {
                type = json_object_get_type(obj);
                switch (type) {
                    case json_type_boolean:
                        if (strcmp(key, "video") == 0) {
                            movie.video = (bool) json_object_get_boolean(obj);
                        } else if (strcmp(key, "adult") == 0) {
                            movie.adult = (bool) json_object_get_boolean(obj);
                        }
                        break;
                    case json_type_double:
                        if (strcmp(key, "vote_average") == 0) {
                            movie.vote_average = json_object_get_double(obj);
                        } else if (strcmp(key, "popularity") == 0) {
                            movie.popularity = json_object_get_double(obj);
                        }
                        break;
                    case json_type_int:
                        if (strcmp(key, "vote_count") == 0) {
                            movie.vote_count = json_object_get_int(obj);
                        } else if (strcmp(key, "id") == 0) {
                            movie.id = json_object_get_int(obj);
                        }
                        break;
                    case json_type_string:
                        if (strcmp(key, "title") == 0) {
                            movie.title = json_object_get_string(obj);
                        } else if (strcmp(key, "poster_path") == 0) {
                            movie.poster_path = json_object_get_string(obj);
                        } else if (strcmp(key, "original_language") == 0) {
                            movie.original_language = json_object_get_string(obj);
                        } else if (strcmp(key, "original_title") == 0) {
                            movie.original_title = json_object_get_string(obj);
                        } else if (strcmp(key, "backdrop_path") == 0) {
                            movie.backdrop_path = json_object_get_string(obj);
                        } else if (strcmp(key, "overview") == 0) {
                            movie.overview = json_object_get_string(obj);
                        } else if (strcmp(key, "release_date") == 0) {
                            movie.release_date = json_object_get_string(obj);
                        }
                        break;
                    case json_type_object:
                        break;
                    case json_type_array:
                        if (strcmp(key, "results") == 0) {
                            parseMovie(obj);
                        }
                        break;
                    default:
                        break;
                }
            }
            movies.push_back(movie);
        }
    }
}
