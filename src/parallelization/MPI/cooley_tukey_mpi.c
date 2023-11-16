#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

// Function to perform the Cooley-Tukey FFT recursively
void cooleyTukeyFFT(complex *X, int N) {
    if (N <= 1) return;

    // Divide step: Divide the array into two halves
    complex even[N/2], odd[N/2];
    for (int i = 0; i < N / 2; i++) {
        even[i] = X[i * 2];
        odd[i] = X[i * 2 + 1];
    }

    // Recur: Compute FFT of the two halves
    cooleyTukeyFFT(even, N / 2);
    cooleyTukeyFFT(odd, N / 2);

    // Combine step
    for (int k = 0; k < N / 2; k++) {
        complex t = cexp(-I * 2 * M_PI * k / N) * odd[k];
        X[k] = even[k] + t;
        X[k + N / 2] = even[k] - t;
    }
}

int main(int argc, char** argv) {
    int rank, size, N = 1024; // Assuming N is the size of the input and is a power of 2
    complex *data, *local_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Initialize data - this should be your actual data
        data = (complex *)malloc(N * sizeof(complex));
        for (int i = 0; i < N; i++) {
            data[i] = i; // Example: initialize with some values
        }
    }

    // Allocate memory for local computations
    int local_N = N / size;
    local_data = (complex *)malloc(local_N * sizeof(complex));

    // Distribute data among processes
    MPI_Scatter(data, local_N, MPI_COMPLEX, local_data, local_N, MPI_COMPLEX, 0, MPI_COMM_WORLD);

    // Perform local FFT
    cooleyTukeyFFT(local_data, local_N);

    // Gather results back to the root process
    MPI_Gather(local_data, local_N, MPI_COMPLEX, data, local_N, MPI_COMPLEX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // The data array now contains the FFT result
        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}