# Fast Fourier Transform (FFT) Algorithms Project

## Overview

This project focuses on the implementation, comparison, and analysis of two distinct Fast Fourier Transform (FFT) algorithms with varying complexities. Each algorithm is chosen to demonstrate different computational trade-offs and use-cases in FFT processing. Our goal is to provide insights into the applicability and performance of these algorithms in diverse scenarios.

## Algorithms

### 1. Cooley-Tukey FFT Algorithm (Radix-2)

#### Complexity:
- **Sequential:** O(N log N)
- **Parallel:** OpenMP can enhance parallel complexity to approximately O(log² N) using multi-core processors, MPI has the potential to reduce complexity to near O(log N) based on inter-node communication efficiency, while CUDA can significantly lower complexity, potentially close to O(log N), thanks to extensive GPU parallelism.

#### Description:
The Cooley-Tukey algorithm, specifically its Radix-2 variant, is a widely-used FFT algorithm. It's a divide-and-conquer approach that efficiently handles sequences of lengths that are powers of 2. This algorithm offers a good balance between ease of implementation and computational efficiency.

#### Use-case:
Ideal for general-purpose applications and educational purposes, particularly when dealing with data sizes that are powers of two.

### 2. Bluestein's FFT Algorithm (Chirp Z-Transform)

#### Complexity:
- **Sequential:** O(N log N) but with a higher constant factor than Cooley-Tukey
- **Parallel:** In Bluestein's FFT algorithm, OpenMP can achieve parallel complexity around O(log² N); MPI could lower it to near O(log N), and CUDA might significantly reduce it to close to O(log N), utilizing the power of GPU parallelism.

#### Description:
This algorithm is tailored for computing DFTs of arbitrary lengths, not just powers of two. It cleverly transforms a DFT into a convolution problem, which is then solved using an FFT of a suitable size. This approach is more intricate than the basic Cooley-Tukey algorithm but provides flexibility in handling diverse data sizes.

#### Use-case:
Applicable in situations where the data length is not a power of two, offering versatility in data processing.

## Project Structure

- **src/**: Source code for each FFT algorithm implementation.
- **docs/**: Documentation and theoretical background of each FFT algorithm.
- **tests/**: Unit tests to ensure the reliability and accuracy of implementations.

## Notes

- **OpenMP/MPI:** More suitable for systems with multiple CPUs or CPU clusters, where inter-process or inter-thread communication is key.
- **CUDA:** Highly efficient for algorithms that can be parallelized at a fine-grained level, like FFT, especially on large data sets.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
