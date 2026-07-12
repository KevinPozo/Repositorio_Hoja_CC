#include <mpi.h>
#include "solver.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (nprocs > 1) {
        ejecutar_mpi(rank, nprocs);
    }

    MPI_Finalize();
    return 0;
}
