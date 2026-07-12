#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>

struct Resultados {
    int conteos[101];
    double promedio;
    int minimo;
    int maximo;
};

std::vector<int> read_file(const std::string& path);
Resultados resolver_colectivas(const std::vector<int>& data, int rank, int nprocs);
Resultados resolver_punto_a_punto(const std::vector<int>& data, int rank, int nprocs);

#endif
