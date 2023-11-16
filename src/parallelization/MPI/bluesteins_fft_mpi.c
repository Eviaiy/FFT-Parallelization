#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

complex double complex_exp(double angle) {
    return cos(angle) + I * sin(angle);
}

// Naive DFT computation for demonstration purposes
void naive_dft(complex double *X, int N, complex double *x, int inverse) {
    double sign = inverse ? 1.0 : -1.0;
    for (int k = 0; k < N; ++k) {
        X[k] = 0;
        for (int n = 0; n < N; ++n) {
            double angle = 2 * PI * k * n / N;
            X[k] += x[n] * complex_exp(sign * angle);
        }
        if (inverse) {
            X[k] /= N;
        }
    }
}

void bluestein_fft(complex double *x, int N, int M, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Allocate memory for chirp signal, its inverse and zero-padded input
    complex double *a = (complex double *)malloc(M * sizeof(complex double));
    complex double *b = (complex double *)malloc(M * sizeof(complex double));
    complex double *c = (complex double *)malloc(M * sizeof(complex double));

    // Generate chirp signal and its inverse
    for (int n = 0; n < N; ++n) {
        a[n] = x[n] * complex_exp(-PI * n * n / N);
        b[n] = complex_exp(PI * n * n / N);
    }
    for (int n = N; n < M; ++n) {
        a[n] = 0;
        b[n] = 0;
    }

    // Perform DFT on chirp and zero-padded input
    naive_dft(c, M, a, 0); // DFT of a
    naive_dft(a, M, b, 0); // DFT of b

    // Multiply the results element-wise
    for (int i = 0; i < M; ++i) {
        c[i] *= a[i];
    }

    // Perform the inverse DFT
    naive_dft(a, M, c, 1);

    // Multiply by chirp signal
    for (int n = 0; n < N; ++n) {
        x[n] = a[n] * complex_exp(-PI * n * n / N);
    }

    free(a);
    free(b);
    free(c);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int N = 16; // FFT size
    int M = 1;  // Find the smallest power of two greater than 2*N-1
    while (M < 2 * N - 1) M *= 2;

    complex double *data = (complex double *)malloc(N * sizeof(complex double));

    // Initialize data
    for (int i = 0; i < N; ++i) {
        data[i] = i + 1; // Example data
    }

    // Perform Bluestein's FFT
    bluestein_fft(data, N, M, MPI_COMM_WORLD);

    // Output the result
    for (int i = 0; i < N; ++i) {
        printf("%f + %fi\n", creal(data[i]), cimag(data[i]));
    }

    free(data);
    MPI_Finalize();
    return 0;
}