#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prev = (rank - 1 + size) % size;
    int next = (rank + 1) % size;

    int value = -1;
    int initial = -1;

    if (size == 1) {
        // Кольцо из одного процесса
        srand((unsigned)time(NULL));
        initial = rand() % 10;
        value = initial + rank; // rank=0
        printf("Rank 0: initial=%d final=%d\n", initial, value);
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        srand((unsigned)time(NULL));
        initial = rand() % 10;
        value = initial;

        // отправляем в кольцо
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        // получаем обратно от последнего
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 0: initial=%d, received_back=%d\n", initial, value);
    } else {
        // принимаем от предыдущего
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // добавляем свой номер
        value += rank;

        // выводим то, что передаем дальше
        printf("Rank %d: passing value=%d\n", rank, value);

        // отправляем следующему
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}