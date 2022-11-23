int isEmpty(world_t **world, int i, int j) {
    if (i < 0 || i >= CONFIG.L || j < 0 || j >= CONFIG.C)
        return 0;
    return world[i][j].occupied == EMPTY;
}

int calcMoveCoelho(world_t **world, int i, int j) {
    int possible = 0;
    int P[4] = {0, 0, 0, 0};

    P[0] = isEmpty(world, i - 1, j);
    P[1] = isEmpty(world, i, j + 1);
    P[2] = isEmpty(world, i + 1, j);
    P[3] = isEmpty(world, i, j - 1);

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

int criaCoelho(world_t worldCell) {
    return worldCell.proc_coelho == CONFIG.GEN_PROC_COELHOS;
}

void moveCoelho(world_t **world, int i, int j) {
    moveTo_t moveTo = calcMoveCoelho(world, i, j);
    world_t worldCell = world[i][j];
    worldCell.proc_coelho += 1;
    worldCell.moved = 1;
    resetCell(world, i, j);

#pragma omp critical
    {
        switch (moveTo) {
            case NORTE:
                world[i - 1][j] = worldCell;
                if (criaCoelho(worldCell)) {
                    world[i][j].occupied = COELHO;
                    world[i - 1][j].proc_coelho = 0;
                }
                break;
            case LESTE:
                world[i][j + 1] = worldCell;
                if (criaCoelho(worldCell)) {
                    world[i][j].occupied = COELHO;
                    world[i][j + 1].proc_coelho = 0;
                }
                break;
            case SUL:
                world[i + 1][j] = worldCell;
                if (criaCoelho(worldCell)) {
                    world[i][j].occupied = COELHO;
                    world[i + 1][j].proc_coelho = 0;
                }
                break;
            case OESTE:
                world[i][j - 1] = worldCell;
                if (criaCoelho(worldCell)) {
                    world[i][j].occupied = COELHO;
                    world[i][j - 1].proc_coelho = 0;
                }
                break;
            case STAY:
                world[i][j] = worldCell;
                break;
            default:
                fprintf(stderr, "Erro no programa: (moveCoelho)\n");
                exit(EXIT_FAILURE);
        }
    }
}

int haveRabbit(world_t **world, int i, int j) {
    if (i < 0 || i >= CONFIG.L || j < 0 || j >= CONFIG.C)
        return 0;
    return world[i][j].occupied == COELHO;
}
