#include "matrix.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>

namespace {
Matrix reference_multiply(const Matrix& left, const Matrix& right) {
    Matrix result{};
    for (std::size_t row = 0; row < dimension; ++row) {
        for (std::size_t col = 0; col < dimension; ++col) {
            std::int64_t sum = 0;
            for (std::size_t k = 0; k < dimension; ++k) {
                sum += static_cast<std::int64_t>(left[row * dimension + k])
                       * right[k * dimension + col];
            }
            result[row * dimension + col] = static_cast<std::int32_t>(sum);
        }
    }
    return result;
}

bool check(const char* name, const Matrix& left, const Matrix& right) {
    const auto expected = reference_multiply(left, right);
    Matrix actual;
    actual.fill(1);
    multiply(left, right, actual);
    const auto [got, want] =
        std::mismatch(actual.begin(), actual.end(), expected.begin());
    if (got != actual.end()) {
        const auto index = got - actual.begin();
        std::cerr << "FAIL " << name << " at (" << index / dimension
                  << ", " << index % dimension << "): got " << *got
                  << ", expected " << *want << '\n';
        return false;
    }
    std::cout << "PASS " << name << '\n';
    return true;
}

void benchmark(const Matrix& left, const Matrix& right) {
    using Clock = std::chrono::steady_clock;
    Matrix output;
    for (int i = 0; i < 3; ++i) {
        multiply(left, right, output);
    }

    std::array<double, 31> samples{};
    std::int64_t checksum = 0;
    for (auto& sample : samples) {
        const auto start = Clock::now();
        multiply(left, right, output);
        const auto end = Clock::now();
        sample = std::chrono::duration<double, std::milli>(end - start).count();
        checksum += std::accumulate(output.begin(), output.end(), std::int64_t{0});
    }
    std::sort(samples.begin(), samples.end());
    std::cout << std::fixed << std::setprecision(3)
              << "Benchmark (" << samples.size() << " runs): median "
              << samples[samples.size() / 2] << " ms, min " << samples.front()
              << " ms, max " << samples.back() << " ms\n"
              << "Checksum: " << checksum << '\n';
}
}  // namespace

int main() {
    Matrix left;
    Matrix right;
    std::mt19937 random(12345);
    std::uniform_int_distribution<std::int32_t> values(-1000, 1000);
    for (auto& value : left) {
        value = values(random);
    }
    for (auto& value : right) {
        value = values(random);
    }
    Matrix zero{};
    Matrix identity{};
    for (std::size_t i = 0; i < dimension; ++i) {
        identity[i * dimension + i] = 1;
    }
    Matrix maximum;
    maximum.fill(1000);

    const bool zero_ok = check("zero matrix", left, zero);
    const bool identity_ok = check("identity matrix", identity, right);
    const bool dense_ok = check("signed random matrices", left, right);
    const bool maximum_ok = check("maximum input values", maximum, maximum);
    if (!zero_ok || !identity_ok || !dense_ok || !maximum_ok) {
        return 1;
    }

    benchmark(left, right);
}
