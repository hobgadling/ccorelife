#ifndef EXECUTION_H
#define EXECUTION_H
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define instruction uint64_t
#define coordinate signed long long
#define GRID_SIZE 255

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
    unsigned char prot;
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

typedef struct write{
    data loc;
    instruction inst;
} write;

data *pointers;
int pointers_length;

data *newPointers;
int newpointers_length;
int follow_flow;

write *writes;
int write_length;

program *owners;

cell grid[GRID_SIZE][GRID_SIZE];

char *getInstruction(instruction);
void fillGrid(void);
void printGrid(void);
void printCell(int,int);
int positive_modulo(int i, int n);
char *getAddresses(instruction);
data getData(int,coordinate,coordinate);
int cellCmp(cell,cell);
int dataCmp(data,data);
exploded_instruction explodeInstruction(instruction);
data getCellAddress(int,coordinate,coordinate);
void saveData(data,data);

//Execution functions
void addInst(data,data,data);
void andInst(data,data,data);
void divInst(data,data,data);
void jmpInst(data);
void jbnInst(data,data);
void jbzInst(data,data);
void jerInst(data,data);
void jesInst(data,data);
void jmfInst(data,data);
void jmtInst(data,data);
void jnrInst(data,data);
void jnsInst(data,data);
void jofInst(data,data);
void jotInst(data,data);
void jrnInst(data,data);
void jrzInst(data,data);
void jsrInst(data,data);
void jssInst(data,data);
void jwrInst(data,data);
void jwsInst(data,data);
void jxnInst(data,data);
void jxzInst(data,data);
void jynInst(data,data);
void jyzInst(data,data);
void modInst(data,data,data);
void moveInst(data,data);
void mulInst(data,data,data);
void notInst(data,data);
void orInst(data,data,data);
void proInst(data);
void ranInst(data,data);
void rseInst(data);
void rsnInst(data);
void rssInst(data);
void rswInst(data);
void splInst(data);
void steInst(data);
void stnInst(data);
void stsInst(data);
void stwInst(data);
void subInst(data,data,data);

//CPU functions
void tick(void);
void executeInstruction(data);

#endif
