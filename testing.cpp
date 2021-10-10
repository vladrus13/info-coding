//
// Created by Vladislav.Kuznetsov on 09.10.2021.
//

#include <iostream>
#include <random>
#include "polar/PolarCoder.h"

const int COUNT_TEST = 500;
const int L = 1;

std::mt19937 rnd;
std::uniform_int_distribution<int> dist(0,1);

bool random_test() {
    int n = 10;
    int info_length = (1 << (n - 1));
    int crc_size = 16;
    double design_epsilon = 0.1;
    PolarCode polarCode(n, design_epsilon, crc_size);
    std::vector<int> read;
    read.reserve(info_length);
    for (int i = 0; i < info_length; i++) {
        read.push_back(dist(rnd));
    }
    std::vector<int> a = polarCode.encode(read);
    std::vector<double> probabilities1{};
    std::vector<double> probabilities0{};
    probabilities1.reserve(a.size());
    probabilities0.reserve(a.size());
    for (uint8_t i: a) {
        probabilities1.push_back(i + 0.1);
        probabilities0.push_back(0.9 - i);
    }
    std::vector<int> decoded = polarCode.decode(probabilities1, probabilities0, L);
    int count = 0;
    for (int i = 0; i < decoded.size(); i++) {
        if (decoded[i] != read[i]) {
            count++;
        }
    }
    return count > 1;
}

int main() {
    for (int i = 0; i < COUNT_TEST; i++) {
        bool random = random_test();
        if (random) {
            return 1;
        }
    }
    return 0;
}