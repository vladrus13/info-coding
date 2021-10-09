//
// Created by Vladislav.Kuznetsov on 09.10.2021.
//

#ifndef INFO_CODING_POLARCODER_H
#define INFO_CODING_POLARCODER_H

#include <vector>
#include <algorithm>

class PolarCode {
private:
    int n;
    int len;
    int block_size;
    int crc;
    double epsilon;
    std::vector<int> frozen;
    std::vector<int> reverse_order;
    std::vector<int> channel;
    std::vector<std::vector<int>> matrix;

public:
    PolarCode(int step, double epsilon, int crc)
            : n(step), len(1 >> (step - 1)), epsilon(epsilon), crc(crc) {
        block_size = (1 << step);
        frozen.resize(block_size, 0);
        reverse_order.resize(block_size);

        for (int i = 0; i < block_size; i++) {
            int reversed = i;
            reverse_order[i] = ((reversed & 1) << (n - 1));
            for (int j = n - 1; j; j--) {
                reversed >>= 1;
                reverse_order[i] -= (reversed & 1) << (j - 1);
            }
        }

        std::vector<double> channel_mistake(block_size, epsilon);
        for (int nest = 0; nest < n; nest++) {
            int inc = 1 << nest;
            for (int j = 0; j < inc; j++) {
                for (int i = 0; i < block_size; i += 2 * inc) {
                    double first_channel = channel_mistake[i + j];
                    double second_channel = channel_mistake[i + j + inc];
                    channel_mistake[i + j] = first_channel + second_channel - first_channel * second_channel;
                    channel_mistake[i + j + inc] = first_channel * second_channel;
                }
            }
        }

        channel.resize(block_size);
        for (int i = 0; i < block_size; i++) {
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
    std::vector<int> decode(const std::vector<double>& p0, const std::vector<double>& p1, int L);
};

#endif //INFO_CODING_POLARCODER_H