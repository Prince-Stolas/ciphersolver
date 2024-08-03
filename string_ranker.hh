#ifndef STRING_RANKER_H
#define STRING_RANKER_H

#include <vector>
#include <string>
#include <fstream>
#include <map>

class string_ranker {
    std::vector<std::string> words;
    std::ifstream file;
    int current_hi_count;
public:
    string_ranker();
    ~string_ranker();
    std::map<int, char>* rank_best(std::string string);
};

#endif