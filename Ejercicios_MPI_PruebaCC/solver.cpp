#include "solver.h"
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <algorithm>

std::vector<int> read_file(const std::string& path) { 
    std::fstream fs(path, std::ios::in); 
    std::string line; 
    std::vector<int> ret; 
    while (std::getline(fs, line)) { 
        ret.push_back(std::stoi(line)); 
    } 
    fs.close(); 
    return ret; 
}

Resultados resolver_colectivas(const std::vector<int>& data, int rank, int nprocs) {
    Resultados res = {};
    int N = data.size();
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = N / nprocs;
    std::vector<int> local_data(local_n);
    MPI_Scatter(data.data(), local_n, MPI_INT, local_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int local_counts[101] = {0};
    double local_sum = 0.0;
    int local_count = 0;
    int local_min = 101;
    int local_max = -1;

    for (int val : local_data) {
        if (val >= 0 && val <= 100) {
            local_counts[val]++;
            local_sum += val;
            local_count++;
            if (val < local_min) local_min = val;
            if (val > local_max) local_max = val;
        }
    }

    MPI_Reduce(local_counts, res.conteos, 101, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double global_sum = 0.0;
    int global_count = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &res.minimo, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &res.maximo, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        res.promedio = (global_count > 0) ? (global_sum / global_count) : 0.0;
    }

    return res;
}

Resultados resolver_punto_a_punto(const std::vector<int>& data, int rank, int nprocs) {
    Resultados res = {};
    int N = data.size();
    if (rank == 0) {
        for (int i = 1; i < nprocs; ++i) {
            MPI_Send(&N, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int local_n = N / nprocs;
    std::vector<int> local_data(local_n);

    if (rank == 0) {
        for (int i = 1; i < nprocs; ++i) {
            MPI_Send(data.data() + i * local_n, local_n, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        std::copy(data.begin(), data.begin() + local_n, local_data.begin());
    } else {
        MPI_Recv(local_data.data(), local_n, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int local_counts[101] = {0};
    double local_sum = 0.0;
    int local_count = 0;
    int local_min = 101;
    int local_max = -1;

    for (int val : local_data) {
        if (val >= 0 && val <= 100) {
            local_counts[val]++;
            local_sum += val;
            local_count++;
            if (val < local_min) local_min = val;
            if (val > local_max) local_max = val;
        }
    }

    if (rank == 0) {
        std::copy(local_counts, local_counts + 101, res.conteos);
        res.minimo = local_min;
        res.maximo = local_max;
        double global_sum = local_sum;
        int global_count = local_count;

        for (int i = 1; i < nprocs; ++i) {
            double r_stats[4];
            int r_counts[101];
            MPI_Recv(r_stats, 4, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(r_counts, 101, MPI_INT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            res.minimo = std::min(res.minimo, (int)r_stats[0]);
            res.maximo = std::max(res.maximo, (int)r_stats[1]);
            global_sum += r_stats[2];
            global_count += (int)r_stats[3];
            for (int j = 0; j < 101; ++j) {
                res.conteos[j] += r_counts[j];
            }
        }

        res.promedio = (global_count > 0) ? (global_sum / global_count) : 0.0;
    } else {
        double local_stats[4] = {(double)local_min, (double)local_max, local_sum, (double)local_count};
        MPI_Send(local_stats, 4, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(local_counts, 101, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    return res;
}
