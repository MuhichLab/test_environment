#include "matrix.hpp"

void multiply(const Matrix& left, const Matrix& right, Matrix& output) {
    output.fill(0);
    for (std::size_t row = 0; row < dimension; ++row) {
        auto* destination = output.data() + row * dimension;
        for (std::size_t k = 0; k < dimension; ++k) {
            const auto factor = left[row * dimension + k];
            const auto* right_row = right.data() + k * dimension;
            for (std::size_t col = 0; col < dimension; ++col) {
                destination[col] += factor * right_row[col];
            }
        }
    }
}
