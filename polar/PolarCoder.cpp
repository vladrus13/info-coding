//
// Created by Vladislav.Kuznetsov on 09.10.2021.
//

#include "PolarCoder.h"

std::vector<int> PolarCode::encode(const std::vector<int> &info) {
    std::vector<int> bits_padded(block_size, 0);
    std::vector<int> returned(block_size);
    for (int i = 0; i < len; i++) {
        bits_padded[channel[i]] = info[i];
    }
    for (int i = len; i < len + crc; ++i) {
        int crc_bit = 0;
        for (int j = 0; j < len; ++j) {
            crc_bit = (crc_bit + matrix[i - len][j] * info[j]) % 2;
        }
        bits_padded[channel[i]] = crc_bit;
    }
    for (int nest = 0; nest < n; ++nest) {
        int inc = 1 << nest;
        for (int j = 0; j < inc; j += 1) {
            for (int i = 0; i < len; i += 2 * inc) {
                bits_padded[i + j] = (bits_padded[i + j] + bits_padded[i + j + inc]) % 2;
            }
        }
    }
    for (int i = 0; i < len; ++i) {
        returned[i] = bits_padded[reverse_order[i]];
    }
    return returned;
}

std::vector<int> PolarCode::decode(const std::vector<double>& p0, const std::vector<double>& p1, int L) {

}
