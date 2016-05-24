#ifndef EXECUTION_H
#define EXECUTION_H
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define instruction uint64_t
#define coordinate signed long long
#define GRID_SIZE 10

typedef struct data{
    coordinate x;
    coordinate y;
} data;

static const data emptyData;

typedef struct program{
    long long id;
    data origin;
} program;

static const program emptyProgram;

typedef struct cell{
    instruction inst;
    unsigned char n;
    unsigned char e;
    unsigned char s;
    unsigned char w;
    program owner;
} cell;

static const cell emptyCell;

typedef struct exploded_instruction{
    char arguments;
    unsigned char inst;
    unsigned char addressing1;
    unsigned long x1;
    unsigned long y1;
    unsigned char addressing2;
    unsigned long x2;
    unsigned long y2;
} exploded_instruction;

data *pointers;
program *owners;

cell grid[GRID_SIZE][GRID_SIZE];

char *getInstruction(instruction);
void fillGrid(void);
void printGrid(void);
char *getAddresses(instruction);
data getData(int,coordinate,coordinate);
int cellCmp(cell,cell);
int dataCmp(data,data);
exploded_instruction explodeInstruction(instruction);
void executeInstruction(data);

#endif
