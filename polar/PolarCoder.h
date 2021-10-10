//
// Created by Vladislav.Kuznetsov on 09.10.2021.
//

#ifndef INFO_CODING_POLARCODER_H
#define INFO_CODING_POLARCODER_H

#include <vector>
#include <algorithm>
#include <stack>

class PolarCode {
private:
    int m;
    int len;
    int n;
    int crc;
    std::vector<int> frozen;
    std::vector<int> reverse_order;
    std::vector<int> channel;
    std::vector<std::vector<int>> matrix;

public:
    PolarCode(int step, double epsilon, int crc)
            : m(step), len(1 << (step - 1)), crc(crc) {
        n = (1 << step);
        frozen.resize(n, 1);
        reverse_order.resize(n);

        for (int i = 0; i < n; i++) {
            int reversed = i;
            reverse_order[i] = ((reversed & 1) << (m - 1));
            for (int j = m - 1; j; j--) {
                reversed >>= 1;
                reverse_order[i] += (reversed & 1) << (j - 1);
            }
        }

        std::vector<double> channel_mistake(n, epsilon);
        for (int nest = 0; nest < m; nest++) {
            int inc = 1 << nest;
            for (int j = 0; j < inc; j++) {
                for (int i = 0; i < n; i += 2 * inc) {
                    double first_channel = channel_mistake[i + j];
                    double second_channel = channel_mistake[i + j + inc];
                    channel_mistake[i + j] = first_channel + second_channel - first_channel * second_channel;
                    channel_mistake[i + j + inc] = first_channel * second_channel;
                }
            }
        }

        channel.resize(n);
        for (int i = 0; i < n; i++) {
            channel[i] = i;
        }
        std::sort(std::begin(channel),
                  std::end(channel),
                  [&](int i1, int i2) {
                      return channel_mistake[reverse_order.at(i1)] < channel_mistake[reverse_order.at(i2)];
                  });
        int record_frozen = len + crc;
        for (int i = 0; i < record_frozen; i++) {
            frozen[channel[i]] = 0;
        }
        matrix.resize(crc);
        for (int bit = 0; bit < crc; bit++) {
            matrix[bit].resize(len);
            for (int info = 0; info < len; info++) {
                matrix[bit][info] = std::rand() % 2;
            }
        }
    }

    std::vector<int> encode(const std::vector<int> &info);

    std::vector<int> decode(const std::vector<double> &p0, const std::vector<double> &p1, int L);

    void initializeDataStructures();

    int LL;

    std::stack<int> inactivePathIndices;
    std::vector<bool> activePath;
    std::vector<std::vector<double *>> arrayPointer_P;
    std::vector<std::vector<int *>> arrayPointer_C;
    std::vector<std::vector<int>> pathIndexToArrayIndex;
    std::vector<std::stack<int>> inactiveArrayIndices;
    std::vector<std::vector<int>> arrayReferenceCount;

    int assignInitialPath();

    int findMostProbablePath();

    void continuePaths_UnfrozenBit(int phi);

    void continuePaths_FrozenBit(int phi);

    void recursivelyUpdateC(int lambda, int phi);

    void killPath(int l);

    int clonePath(int l);

    void recursivelyCalcP(int lambda, int phi);

    int *getArrayPointer_C(int lambda, int l);

    double *getArrayPointer_P(int lambda, int l);

    bool pathIndexInactive(int l);

    template<class T>
    int getArrayPointer(int lambda, int l, const std::vector<std::vector<T*>> &pointer);
};

#endif //INFO_CODING_POLARCODER_H