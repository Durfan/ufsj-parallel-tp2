int calcMoveRaposa(world_t **world, int i, int j) {
    int possible = 0;
    int P[4] = {0, 0, 0, 0};

    P[0] = haveRabbit(world, i - 1, j);
    P[1] = haveRabbit(world, i, j + 1);
    P[2] = haveRabbit(world, i + 1, j);
    P[3] = haveRabbit(world, i, j - 1);

    for (int i = 0; i < 4; i++)
        possible += P[i];

    if (possible == 0) {
        P[0] = isEmpty(world, i - 1, j);
        P[1] = isEmpty(world, i, j + 1);
        P[2] = isEmpty(world, i + 1, j);
        P[3] = isEmpty(world, i, j - 1);
    }

    for (int i = 0; i < 4; i++)
        possible += P[i];

    if (possible == 0) return STAY;

    int choice = (CONFIG.GEN + i + j) % possible;
    for (int i = 0; i < 4; i++) {
        if (P[i] == 0 && choice == i) {
            for (int j = i; j < 4; j++) {
                if (P[j] == 1) {
                    choice = j;
                    break;
                }
            }
        }
    }

    return choice;
}

int criaRaposa(world_t worldCell) {
    return worldCell.proc_raposa == CONFIG.GEN_PROC_RAPOSAS;
}

int comeCoelho(world_t worldCell) {
    return worldCell.occupied == COELHO;
}

int mataRaposa(world_t worldCell) {
    return worldCell.comida_raposa == CONFIG.GEN_COMIDA_RAPOSAS;
}

void moveRaposa(world_t **world, int i, int j) {
    moveTo_t moveTo = calcMoveRaposa(world, i, j);
    world_t worldCell = world[i][j];
    worldCell.proc_raposa += 1;
    worldCell.comida_raposa += 1;
    worldCell.moved = 1;
    resetCell(world, i, j);

#pragma omp critical
    {
        switch (moveTo) {
            case NORTE:
                if (!mataRaposa(worldCell)) {
                    if (comeCoelho(world[i - 1][j])) {
                        world[i - 1][j] = worldCell;
                        world[i - 1][j].comida_raposa = 0;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i - 1][j].proc_raposa = 0;
                        }
                    } else {
                        world[i - 1][j] = worldCell;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i - 1][j].proc_raposa = 0;
                        }
                    }
                }
                break;
            case LESTE:
                if (!mataRaposa(worldCell)) {
                    if (comeCoelho(world[i][j + 1])) {
                        world[i][j + 1] = worldCell;
                        world[i][j + 1].comida_raposa = 0;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i][j + 1].proc_raposa = 0;
                        }
                    } else {
                        world[i][j + 1] = worldCell;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i][j + 1].proc_raposa = 0;
                        }
                    }
                }
                break;
            case SUL:
                if (!mataRaposa(worldCell)) {
                    if (comeCoelho(world[i + 1][j])) {
                        world[i + 1][j] = worldCell;
                        world[i + 1][j].comida_raposa = 0;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i + 1][j].proc_raposa = 0;
                        }
                    } else {
                        world[i + 1][j] = worldCell;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i + 1][j].proc_raposa = 0;
                        }
                    }
                }
                break;
            case OESTE:
                if (!mataRaposa(worldCell)) {
                    if (comeCoelho(world[i][j - 1])) {
                        world[i][j - 1] = worldCell;
                        world[i][j - 1].comida_raposa = 0;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i][j - 1].proc_raposa = 0;
                        }
                    } else {
                        world[i][j - 1] = worldCell;
                        if (criaRaposa(worldCell)) {
                            world[i][j].occupied = RAPOSA;
                            world[i][j - 1].proc_raposa = 0;
                        }
                    }
                }
                break;
            case STAY:
                world[i][j] = worldCell;
                break;
            default:
                fprintf(stderr, "Erro no programa: (moveRaposa)\n");
                exit(EXIT_FAILURE);
        }
    }
}
