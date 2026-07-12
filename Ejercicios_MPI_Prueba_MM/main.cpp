#include <mpi.h>
#include <iostream>
#include <vector>
#include <fmt/core.h>
#include "solver.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int n = 0;
    int m = 0;

    std::vector<int> U;
    std::vector<int> V;
    std::vector<int> global_matrix;

    if (rank == 0) {
        U = {1, 2, 3, 4, 5, 6, 7, 8};
        V = {2, 3, 4, 5, 6, 7};

        n = U.size();
        m = V.size();
        global_matrix.resize(n * m);

        fmt::print("Vector U: ");
        for (int val : U) fmt::print("{} ", val);
        fmt::print("\n");

        fmt::print("Vector V: ");
        for (int val : V) fmt::print("{} ", val);
        fmt::print("\n\n");
    }

    resolver_producto_externo(U, V, global_matrix, rank, nprocs);

    if (rank == 0) {
        fmt::print("Producto Externo:\n");
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                fmt::print("{:>4}", global_matrix[i * m + j]);
            }
            fmt::print("\n");
        }
        fmt::print("\n");
    }

    MPI_Finalize();
    return 0;
}
