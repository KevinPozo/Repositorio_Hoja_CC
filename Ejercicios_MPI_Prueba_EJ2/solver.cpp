#include "solver.h"
#include <mpi.h>
#include <random>
#include <fmt/core.h>

void ejecutar_mpi(int rank, int nprocs) {
    int valor = 0;

    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(100, 999);
        valor = dis(gen);

        fmt::print("RANK_0 valor={}\n", valor);
        
        fmt::print("RANK_0 enviando a RANK_1\n");
        MPI_Send(&valor, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&valor, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fmt::print("RANK_0 recibido de RANK_{} valor={}\n", nprocs - 1, valor);
    } 
    else {
        int origen = rank - 1;
        int destino = (rank + 1) % nprocs;

        MPI_Recv(&valor, 1, MPI_INT, origen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fmt::print("RANK_{} recibido desde RANK_{} valor={}\n", rank, origen, valor);

        valor++;
        fmt::print("RANK_{} valor={}\n", rank, valor);

        fmt::print("RANK_{} enviando a RANK_{}\n", rank, destino);
        MPI_Send(&valor, 1, MPI_INT, destino, 0, MPI_COMM_WORLD);
    }
}
