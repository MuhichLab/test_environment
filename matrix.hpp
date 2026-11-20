#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

inline constexpr std::size_t dimension = 256;
using Matrix = std::array<std::int32_t, dimension * dimension>;

// Inputs have entries in [-1000, 1000]. The output must be a separate matrix.
void multiply(const Matrix& left, const Matrix& right, Matrix& output);
