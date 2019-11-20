//
// Created by cpasjuste on 20/11/2019.
//

#ifndef PSCRAP_LEVENSHTEIN_H
#define PSCRAP_LEVENSHTEIN_H

#include <string>

namespace pscrap {

    class Levenshtein {

    public:
        static size_t distance(const std::string &s1, const std::string &s2);
    };
}

#endif //PSCRAP_LEVENSHTEIN_H
