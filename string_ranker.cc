#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <map>

#include "string_ranker.hh"

string_ranker::string_ranker() {
    file.open("wordlist.4");
    
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file" << "\n";
        exit(1);
    }

    std::string word;
    while (std::getline(file, word)) words.push_back(word);

    current_hi_count = 0;
}

string_ranker::~string_ranker() {
    file.close();
}

std::map<int, char>* string_ranker::rank_best(std::string string) {
    auto index_char_map = new std::map<int, char>;

    std::vector<int> word_matches;
    word_matches.resize(words.size());
    std::transform(words.cbegin(), words.cend(), word_matches.begin(),
        [&](std::string word){
            if (word.size() > string.size()) return 0;
            int count = 0;
            for (int i = 0; (unsigned long) i < string.length() - word.length() + 1; i++)
                if (string.substr(i, word.length()) == word){
                    count++;
                    (*index_char_map)[i] = string[i];
                }
            return count;
        });

    int count = std::reduce(word_matches.cbegin(), word_matches.cend());
    if (count > current_hi_count) {
        current_hi_count = count;
        std::cout << count
                  << ": "
                  << string
                  << "\n";
    }

    return index_char_map;
}