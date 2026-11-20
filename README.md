# test_environment

Multiplies two row-major 256 × 256 matrices with signed integer entries in
the range −1000 to 1000. This bound keeps every product and partial sum within
the signed 32-bit range.

From this directory, build and run with:

```sh
g++ -std=c++17 -O3 -march=native -Wall -Wextra -Wpedantic matrix.cpp main.cpp -o matrix_bench
./matrix_bench
```

The executable compares all output entries against a separate dot-product
implementation with 64-bit accumulation. It exits with status 1 if any
correctness check fails. After the checks pass, it warms up three times and
reports the median, minimum, and maximum wall-clock time over 31
multiplications. Timing includes clearing the output matrix and excludes
input generation, correctness checks, and checksum calculation.
