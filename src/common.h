#ifndef _COMMON_H
#define _COMMON_H

typedef struct {
    int GEN_PROC_COELHOS;
    int GEN_PROC_RAPOSAS;
    int GEN_COMIDA_RAPOSAS;
    int N_GEN;
    int GEN;
    int L;
    int C;
    int N;
} config_t;

config_t CONFIG;

typedef enum {
    STAY = -1,
    NORTE,
    LESTE,
    SUL,
    OESTE
} moveTo_t;

typedef enum {
    ERROR = -1,
    EMPTY,
    ROCHA,
    COELHO,
    RAPOSA
} occupied_t;

typedef struct {
    occupied_t occupied;
    int proc_coelho;
    int proc_raposa;
    int comida_raposa;
    int moved;
} world_t;

static struct {
    occupied_t val;
    const char *str;
} bicho[] = {
    {ROCHA, "ROCHA"},
    {COELHO, "COELHO"},
    {RAPOSA, "RAPOSA"}};

#endif