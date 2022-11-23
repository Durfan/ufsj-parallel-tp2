const char *enum2coord(int i) {
    switch (i) {
        case NORTE:
            return "NORTE";
        case LESTE:
            return "LESTE";
        case SUL:
            return "SUL";
        case OESTE:
            return "OESTE";
        default:
            return "ERROR";
    }
}

int bicho2enum(const char *str) {
    for (long unsigned int i = 0; i < sizeof(bicho) / sizeof(bicho[0]); ++i)
        if (!strcmp(str, bicho[i].str))
            return bicho[i].val;
    return ERROR;
}

world_t **createWorld() {
    world_t **world = (world_t **)malloc(CONFIG.L * sizeof(world_t *));
    for (int i = 0; i < CONFIG.L; i++)
        world[i] = (world_t *)malloc(CONFIG.C * sizeof(world_t));

    for (int i = 0; i < CONFIG.L; i++)
        for (int j = 0; j < CONFIG.C; j++)
            world[i][j] = (world_t){EMPTY, 0, 0, 0, 0};

    return world;
}

world_t **getWorld(const char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Falha ao abrir arquivo.");
        exit(EXIT_FAILURE);
    }

    char buffer[8];
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int l, c;

    read = getline(&line, &len, fp);
    sscanf(line, "%d %d %d %d %d %d %d", &CONFIG.GEN_PROC_COELHOS,
           &CONFIG.GEN_PROC_RAPOSAS, &CONFIG.GEN_COMIDA_RAPOSAS,
           &CONFIG.N_GEN, &CONFIG.L, &CONFIG.C, &CONFIG.N);

    world_t **world = createWorld();

    while ((read = getline(&line, &len, fp)) != -1) {
        sscanf(line, "%s %d %d", buffer, &l, &c);
        world[l][c].occupied = bicho2enum(buffer);
        if (world[l][c].occupied == ERROR) {
            fprintf(stderr, "Erro ao ler arquivo de configuração.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (line)
        free(line);
    fclose(fp);

    return world;
}

void resetCell(world_t **world, int i, int j) {
    world[i][j].occupied = EMPTY;
    world[i][j].proc_coelho = 0;
    world[i][j].proc_raposa = 0;
    world[i][j].comida_raposa = 0;
    world[i][j].moved = 0;
}

void freeWorld(world_t **world) {
    for (int i = 0; i < CONFIG.L; i++)
        free(world[i]);
    free(world);
}

void printWorld(world_t **world) {
    const char *file = "output.txt";
    FILE *fp = fopen(file, "a+");
    if (fp == NULL) {
        perror("Falha ao abrir arquivo.");
        exit(EXIT_FAILURE);
    }

    printf("Geração %d\n", CONFIG.GEN);
    fprintf(fp, "Geração %d\n", CONFIG.GEN);

    for (int i = 0; i < CONFIG.L; i++) {
        for (int j = 0; j < CONFIG.C; j++) {
            switch (world[i][j].occupied) {
                case ROCHA:
                    printf("* ");
                    fprintf(fp, "* ");
                    break;
                case COELHO:
                    printf("C ");
                    fprintf(fp, "C ");
                    break;
                case RAPOSA:
                    printf("R ");
                    fprintf(fp, "R ");
                    break;
                case EMPTY:
                    printf("- ");
                    fprintf(fp, "- ");
                    break;
                case ERROR:
                    printf("E ");
                    fprintf(fp, "E ");
                    break;
                default:
                    fprintf(stderr, "Erro no programa: (printWorld)\n");
                    exit(EXIT_FAILURE);
            }
        }
        printf("\n");
        fprintf(fp, "\n");
    }
    printf("\n");
    fprintf(fp, "\n");
    fclose(fp);
}

void resetMoves(world_t **world) {
    for (int i = 0; i < CONFIG.L; i++)
        for (int j = 0; j < CONFIG.C; j++)
            world[i][j].moved = 0;
}
