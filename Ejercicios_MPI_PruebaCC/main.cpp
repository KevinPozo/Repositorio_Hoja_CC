#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <fmt/core.h>
#include "solver.h"

void imprimir_tabla(const Resultados& res) {
    fmt::print("------------------\n");
    fmt::print("| Valor | Conteo |\n");
    fmt::print("------------------\n");
    for (int i = 0; i <= 100; ++i) {
        if (res.conteos[i] > 0) {
            fmt::print("| {:>5} | {:>6} |\n", i, res.conteos[i]);
        }
    }
    fmt::print("------------------\n");
    fmt::print("Promedio: {:.2f}\n", res.promedio);
    fmt::print("Minimo: {}\n", res.minimo);
    fmt::print("Maximo: {}\n\n", res.maximo);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> data;
    if (rank == 0) {
        data = read_file("datos.txt");

        int N = data.size();
        int remainder = N % nprocs;
        if (remainder != 0) {
            int padding = nprocs - remainder;
            for (int i = 0; i < padding; ++i) {
                data.push_back(-1);
            }
        }
    }

    Resultados res_col = resolver_colectivas(data, rank, nprocs);
    if (rank == 0) {
        fmt::print("Metodo de Colectivas: \n");
        imprimir_tabla(res_col);
    }

    Resultados res_p2p = resolver_punto_a_punto(data, rank, nprocs);
    if (rank == 0) {
        fmt::print("Metodo de Punto a Punto: \n");
        imprimir_tabla(res_p2p);
    }

    MPI_Finalize();
    return 0;
}
