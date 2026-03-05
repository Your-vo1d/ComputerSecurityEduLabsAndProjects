#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static double frand_0_9(void) {
    return (double)(rand() % 10);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = size; 

    int rows_per_proc = N / size;

    double* A_global = NULL;
    double* x_global = (double*)malloc(N * sizeof(double));
    double* y_global = (double*)malloc(N * sizeof(double));

    // локальные куски
    double* A_local = (double*)malloc((size_t)rows_per_proc * N * sizeof(double));
    double* y_local = (double*)malloc(rows_per_proc * sizeof(double));

    if (!x_global || !y_global || !A_local || !y_local) {
        fprintf(stderr, "Rank %d: memory allocation failed\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 2);
    }

    if (rank == 0) {
        A_global = (double*)malloc((size_t)N * N * sizeof(double));
        if (!A_global) {
            fprintf(stderr, "Rank 0: memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 2);
        }

        srand((unsigned)time(NULL));

        // заполняем A и x случайными числами 0..9
        for (int i = 0; i < N * N; ++i) A_global[i] = frand_0_9();
        for (int i = 0; i < N; ++i)     x_global[i] = frand_0_9();
    }

    // 1) разослать x_global всем
    MPI_Bcast(x_global, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 2) распределить строки A_global по процессам
    MPI_Scatter(
        A_global, rows_per_proc * N, MPI_DOUBLE,
        A_local,  rows_per_proc * N, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    // 3) локальное умножение: y_local = A_local * x_global
    for (int i = 0; i < rows_per_proc; ++i) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += A_local[i * N + j] * x_global[j];
        }
        y_local[i] = sum;
    }

    // 4) собрать y полностью на каждом процессе
    MPI_Allgather(
        y_local, rows_per_proc, MPI_DOUBLE,
        y_global, rows_per_proc, MPI_DOUBLE,
        MPI_COMM_WORLD
    );

    // Проверка на rank 0 (последовательное умножение)
    if (rank == 0) {
        double* y_seq = (double*)malloc(N * sizeof(double));
        if (!y_seq) {
            fprintf(stderr, "Rank 0: memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 2);
        }

        for (int i = 0; i < N; ++i) {
            double sum = 0.0;
            for (int j = 0; j < N; ++j) {
                sum += A_global[i * N + j] * x_global[j];
            }
            y_seq[i] = sum;
        }

        // сравнение
        double max_abs_diff = 0.0;
        for (int i = 0; i < N; ++i) {
            double d = fabs(y_seq[i] - y_global[i]);
            if (d > max_abs_diff) max_abs_diff = d;
        }

        printf("Check on rank 0: max |y_seq - y_parallel| = %.12g\n", max_abs_diff);

        // при желании — вывести y
        printf("y (parallel) = [");
        for (int i = 0; i < N; ++i) {
            printf("%s%.2f", (i ? ", " : ""), y_global[i]);
        }
        printf("]\n");

        free(y_seq);
    }


    free(A_local);
    free(y_local);
    free(x_global);
    free(y_global);
    if (rank == 0) free(A_global);

    MPI_Finalize();
    return 0;
}