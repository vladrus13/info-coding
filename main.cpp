#include <iostream>
#include "polar/PolarCoder.h"

int main() {
    int n = 11;
    int info_length = (1 << (n - 1));
    int crc_size = 16;
    double design_epsilon = 0.1;
    PolarCode polarCode(n, design_epsilon, crc_size);
    std::vector<int> a = polarCode.encode(std::vector<int>(info_length, 0));
    std::vector<double> probabilities1{};
    std::vector<double> probabilities0{};
    probabilities1.reserve(a.size());
    probabilities0.reserve(a.size());
    for (int i: a) {
        probabilities1.push_back(i + 0.1);
        probabilities0.push_back(0.9 - i);
    }
    std::vector<int> decoded = polarCode.decode(probabilities1, probabilities0, 32);
    for (int i: decoded) {
        std::cout << (int) i << ' ';
    }
    return 0;
}
