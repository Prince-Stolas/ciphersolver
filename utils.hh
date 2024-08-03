#ifndef UTILS_HH
#define UTILS_HH

#include <iostream>
#include <vector>

template<typename T>
void print_vector(std::vector<T>* vec) {
    std::cout << "[";
    for (T t : *vec) {
        std::cout << t << ", ";
    }
    std::cout << "]\n";
}

#endif