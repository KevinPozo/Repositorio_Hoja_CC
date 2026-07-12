#include "solver.h"
#include <mpi.h>

void resolver_producto_externo(const std::vector<int>& U, const std::vector<int>& V, std::vector<int>& global_matrix, int rank, int nprocs) {
    int n = U.size();
    int m = V.size();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / nprocs;
    std::vector<int> local_u(local_n);
    std::vector<int> local_V(m);

    if (rank == 0) {
        local_V = V;
    }

    MPI_Scatter(U.data(), local_n, MPI_INT, local_u.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(local_V.data(), m, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_matrix(local_n * m);
    for (int i = 0; i < local_n; ++i) {
        for (int j = 0; j < m; ++j) {
            local_matrix[i * m + j] = local_u[i] * local_V[j];
        }
    }

    MPI_Gather(local_matrix.data(), local_n * m, MPI_INT, global_matrix.data(), local_n * m, MPI_INT, 0, MPI_COMM_WORLD);
}
