#include <limits.h>

#include <iostream>
#include <array>
#include <set>
#include <vector>
#include <thread>
#include <map>

#include "string_ranker.hh"
#include "solver.hh"

void decrypt_once(std::vector<int> num_vec, std::vector<int> tokens, std::vector<char> target_vec, std::map<int, char>* cribs) {
    std::vector<std::thread*> threads;

    string_ranker ranker;
    decrypt::substitute_all<int, char>(cribs, num_vec, tokens, target_vec,
        [&](decrypt::substitution<int, char>* sub) {
            std::string str(sub->result->cbegin(), sub->result->cend());
            //std::cout << "Key: ";
            //for (auto& e : *(sub->key)) {
            //    std::cout << e.first << " => ";
            //    for (auto& num : *e.second) {
            //        std::cout << num << ",";
            //    }
            //}
            std::cout << str << "\n";
            auto new_found = ranker.rank_best(str);
            if (new_found != nullptr) {
                auto new_cribs = new std::map<int, char>;
                new_cribs->insert(cribs->begin(), cribs->end());
                for (auto e : *new_found) {
                    int old_value = num_vec[e.first];

                    if (!new_cribs->contains(old_value)) {
                        (*new_cribs)[old_value] = e.second;
                    }
                }

                std::cout << "Created new thread!" << "\n";
                threads.push_back(new std::thread([&](){
                    decrypt_once(num_vec, tokens, target_vec, new_cribs);
                }));
            }
        }
    );

    for (auto t : threads) {
        while (t->joinable()) {}
        delete t;
    }

    std::cout << "Thread Finished!" << "\n";
}

int main(int argc, char** argv) {
    //std::vector<int> num_vec = {71, 194, 38, 1701, 89, 76, 11, 83, 1629, 48, 94, 63, 132, 16, 111, 95, 84, 341, 975, 14, 40, 64, 27, 81, 139, 213, 63, 90, 1120, 8, 15, 3, 126, 2018, 40, 74, 758, 485, 604, 230, 436, 664, 582, 150, 251, 284, 308, 231, 124, 211, 486, 225, 401, 370, 11, 101, 305, 139, 189, 17, 33, 88, 208, 193, 145, 1, 94, 73, 416, 918, 263, 28, 500, 538, 356, 117, 136, 219, 27, 176, 130, 10, 460, 25, 485, 18, 436, 65, 84, 200, 283, 118, 320, 138, 36, 416, 280, 15, 71, 224, 961, 44, 16, 401, 39, 88, 61, 304, 12, 21, 24, 283, 134, 92, 63, 246, 486, 682, 7, 219, 184, 360, 780, 18, 64, 463, 474, 131, 160, 79, 73, 440, 95, 18, 64, 581, 34, 69, 128, 367, 460, 17, 81, 12, 103, 820, 62, 116, 97, 103, 862, 70, 60, 1317, 471, 540, 208, 121, 890, 346, 36, 150, 59, 568, 614, 13, 120, 63, 219, 812, 2160, 1780, 99, 35, 18, 21, 136, 872, 15, 28, 170, 88, 4, 30, 44, 112, 18, 147, 436, 195, 320, 37, 122, 113, 6, 140, 8, 120, 305, 42, 58, 461, 44, 106, 301, 13, 408, 680, 93, 86, 116, 530, 82, 568, 9, 102, 38, 416, 89, 71, 216, 728, 965, 818, 2, 38, 121, 195, 14, 326, 148, 234, 18, 55, 131, 234, 361, 824, 5, 81, 623, 48, 961, 19, 26, 33, 10, 1101, 365, 92, 88, 181, 275, 346, 201, 206, 86, 36, 219, 324, 829, 840, 64, 326, 19, 48, 122, 85, 216, 284, 919, 861, 326, 985, 233, 64, 68, 232, 431, 960, 50, 29, 81, 216, 321, 603, 14, 612, 81, 360, 36, 51, 62, 194, 78, 60, 200, 314, 676, 112, 4, 28, 18, 61, 136, 247, 819, 921, 1060, 464, 895, 10, 6, 66, 119, 38, 41, 49, 602, 423, 962, 302, 294, 875, 78, 14, 23, 111, 109, 62, 31, 501, 823, 216, 280, 34, 24, 150, 1000, 162, 286, 19, 21, 17, 340, 19, 242, 31, 86, 234, 140, 607, 115, 33, 191, 67, 104, 86, 52, 88, 16, 80, 121, 67, 95, 122, 216, 548, 96, 11, 201, 77, 364, 218, 65, 667, 890, 236, 154, 211, 10, 98, 34, 119, 56, 216, 119, 71, 218, 1164, 1496, 1817, 51, 39, 210, 36, 3, 19, 540, 232, 22, 141, 617, 84, 290, 80, 46, 207, 411, 150, 29, 38, 46, 172, 85, 194, 39, 261, 543, 897, 624, 18, 212, 416, 127, 931, 19, 4, 63, 96, 12, 101, 418, 16, 140, 230, 460, 538, 19, 27, 88, 612, 1431, 90, 716, 275, 74, 83, 11, 426, 89, 72, 84, 1300, 1706, 814, 221, 132, 40, 102, 34, 868, 975, 1101, 84, 16, 79, 23, 16, 81, 122, 324, 403, 912, 227, 936, 447, 55, 86, 34, 43, 212, 107, 96, 314, 264, 1065, 323, 428, 601, 203, 124, 95, 216, 814, 2906, 654, 820, 2, 301, 112, 176, 213, 71, 87, 96, 202, 35, 10, 2, 41, 17, 84, 221, 736, 820, 214, 11, 60, 760};
    //hello this is funny
    //h   e   l o    t    i s       f  u y    n
    //231 328 5 1967 2024 2 4712874 27 0 1947 1

    //off hello young hj

    //h   e   l o    t    i s       a  r y
    //231 328 5 1967 2024 2 4712874 27 0 1947
    //std::vector<int> num_vec = {231, 328, 5, 5, 1967, 2024, 231, 2, 4712874, 2, 4712874, 27, 2024, 328, 4712874, 2024, 2024, 1967, 2024, 0, 1947, 2024, 231, 2, 4712874};
    std::vector<int> num_vec = {231, 328, 5, 5, 1967, 2024, 231, 2, 4712874, 2, 4712874, 27, 0, 1, 1, 1947};
    std::vector<int> tokens = num_vec;
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

    std::sort(tokens.begin(), tokens.end());    
    tokens.erase(std::unique(tokens.begin(), tokens.end()), tokens.end());

    auto cribs = new std::map<int, char>;
    (*cribs)[231] = 'h';
    (*cribs)[328] = 'e';
    (*cribs)[5] = 'l';
    (*cribs)[1967] = 'o';
    (*cribs)[2024] = 't';
    (*cribs)[2] = 'i';
    //(*cribs)[4712874] = 's';
    (*cribs)[1] = 'n';
    //(*cribs)[1947] = 'y';

    std::vector<char> target_vec (alphabet.begin(), alphabet.end());

    decrypt_once(num_vec, tokens, target_vec, cribs);

    return 0;
}