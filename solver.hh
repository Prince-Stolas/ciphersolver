#include <map>
#include <vector>
#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include <ranges>
#include <iostream>

#include "utils.hh"

namespace decrypt {

template<typename T, typename R>
class substitution {
public:
    std::map<R, std::vector<T>*>* key;
    std::vector<R>* result;

    substitution(std::map<R, std::vector<T>*>* k, std::vector<R>* r)
        : key(k),
            result(r) {}
    
    ~substitution() {
        for (const auto& k : *key) delete k.second;
        delete key;
        delete result;
    }
};

template<typename T, typename R>
substitution<T, R>* _run_substitution_once(
        std::map<T, R>* cribs,
        std::vector<T> src,
        std::vector<T> current,
        std::vector<R> target_vec) {

    //std::cout << "Target Vec: ";
    //print_vector(&target_vec);

    auto key = new std::map<R, std::vector<T>*>;
    std::map<T, R> mapping;
    for (const auto& crib : *cribs) {
        if (!key->contains(crib.second)) {
            (*key)[crib.second] = new std::vector<T>;
        }
        (*key)[crib.second]->push_back(crib.first);
        mapping[crib.first] = crib.second;
    }

    for (int i = 0; (unsigned long) i < current.size(); i++) {
        auto t = current[i];
        R* r = nullptr;
        if (cribs->contains(t)) {
            r = &(*cribs)[t];
        } else {
            r = &target_vec[i % target_vec.size()];
        }

        if (!key->contains(*r)) {
            (*key)[*r] = new std::vector<T>;
        }
        (*key)[*r]->push_back(t);
        mapping[t] = *r;
    }
    auto result = new std::vector<R>;
    for (const auto& elem : src) {
        result->push_back(mapping[elem]);
    }
    
    return new substitution<T, R>(key, result);
}

template<typename T, typename R>
void substitute_all(
        std::map<T, R>* cribs,
        std::vector<T> src,
        std::vector<T> source_vec,
        std::vector<R> target_vec,
        std::function<void(substitution<T, R>*)> func) {

    for (const auto& e : *cribs) {
        target_vec.erase(std::remove(target_vec.begin(), target_vec.end(), e.second), target_vec.end());
        source_vec.erase(std::remove(source_vec.begin(), source_vec.end(), e.first), source_vec.end());
    }

    std::sort(source_vec.begin(), source_vec.end());

    for (const auto& e : source_vec) {
        std::cout << e << ", ";
    }
    std::cout << "\n";

    do {
        substitution<int, char>* sub = _run_substitution_once(cribs, src, source_vec, target_vec);
        func(sub);
        delete sub;
    } while (
        source_vec.size() >= target_vec.size() ?
            std::next_permutation(source_vec.begin(), source_vec.end())
        :   std::next_permutation(target_vec.begin(), target_vec.end())
    );
}

}
