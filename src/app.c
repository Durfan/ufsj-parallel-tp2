#define _GNU_SOURCE
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "world.h"
#include "rabbit.h"
#include "fox.h"

void simulate(world_t **world) {
    CONFIG.GEN = 0;
    int nthreads, tid, i, chunk;

    printWorld(world);

    while (CONFIG.GEN < CONFIG.N_GEN) {
        #pragma omp parallel shared(world, nthreads, chunk) private(i, tid)
        {
            tid = omp_get_thread_num();
            if (tid == 0) {
                nthreads = omp_get_num_threads();
                chunk = (CONFIG.L * CONFIG.C) / nthreads;
            }
            printf("Thread %d iniciando...\n", tid);

            #pragma omp for schedule(dynamic, chunk)
            for (i = 0; i < CONFIG.L; i++) {
                for (int j = 0; j < CONFIG.C; j++) {
                    if (world[i][j].occupied == COELHO && !world[i][j].moved) {
                        moveCoelho(world, i, j);
                    }
                }
            }
            #pragma omp for schedule(dynamic, chunk)
            for (i = 0; i < CONFIG.L; i++) {
                for (int j = 0; j < CONFIG.C; j++) {
                    if (world[i][j].occupied == RAPOSA && !world[i][j].moved) {
                        moveRaposa(world, i, j);
                    }
                }
            }
        }
        CONFIG.GEN++;
        resetMoves(world);
        printWorld(world);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo de configuração>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    world_t **world = getWorld(argv[1]);
    simulate(world);
    freeWorld(world);

    return 0;
}