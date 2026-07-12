#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

void resolver_producto_externo(const std::vector<int>& U, const std::vector<int>& V, std::vector<int>& global_matrix, int rank, int nprocs);

#endif
