#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define instruction uint64_t
#define coordinate signed long long
#define GRID_SIZE 10

typedef struct data{
    coordinate x;
    coordinate y;
} data;

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
exploded_instruction explodeInstruction(instruction);
void executeInstruction(data);

int main(){
    int i,j;

    srand(time(NULL));

    fillGrid();

    printGrid();

    return 0;
}

void fillGrid(){
    int i,j;
    instruction num;

    for(i = 0; i < GRID_SIZE; i++){
        for(j = 0; j < GRID_SIZE; j++){
            if(rand() % 3 == 0){
                num = rand();
                num = (num << 33) | rand();
                grid[i][j].inst = num;
                grid[i][j].n = rand() & 1;
                grid[i][j].e = rand() & 1;
                grid[i][j].w = rand() & 1;
                grid[i][j].s = rand() & 1;
            } else{
                grid[i][j] = emptyCell;
            }
        }
    }
}

void printGrid(){
    int i,j;

    for(i = 0; i < GRID_SIZE; i++){
        for(j = 0; j < GRID_SIZE; j++){
            printf("%ix%i: ",i,j);
            if(cellCmp(grid[i][j],emptyCell) == 0){
                printf("\n");
            } else {
                printf("%s, ", getInstruction(grid[i][j].inst));
                if(strcmp(getInstruction(grid[i][j].inst),"nop") != 0){
                    printf("%s ", getAddresses(grid[i][j].inst));
                }
                if(grid[i][j].n == 1){
                    printf("N");
                }
                if(grid[i][j].e == 1){
                    printf("E");
                }
                if(grid[i][j].w == 1){
                    printf("W");
                }
                if(grid[i][j].s == 1){
                    printf("S");
                }
            }
            printf("\n");
        }
    }
}

char *getInstruction(instruction i){
    switch(i >> 63){
        case 0:
            switch((i >> 59) & 0xf){
                case 0:
                    return "data";
                case 1:
                    return "jmp";
                case 2:
                    return "not";
                case 3:
                    return "prot";
                case 4:
                    return "rstn";
                case 5:
                    return "rste";
                case 6:
                    return "rsts";
                case 7:
                    return "rstw";
                case 8:
                    return "setn";
                case 9:
                    return "sete";
                case 0xa:
                    return "sets";
                case 0xb:
                    return "setw";
                case 0xc:
                    return "split";
                default:
                    return "nop";
            }
            break;
        case 1:
            switch((i >> 58) & 0x1f){
                case 0x00:
                    return "add";
                case 0x01:
                    return "and";
                case 0x02:
                    return "div";
                case 0x03:
                    return "jmpxz";
                case 0x04:
                    return "jmpxn";
                case 0x05:
                    return "jmpyz";
                case 0x06:
                    return "jmpyn";
                case 0x07:
                    return "jmpbz";
                case 0x08:
                    return "jmpbn";
                case 0x09:
                    return "jmpnr";
                case 0x0A:
                    return "jmpns";
                case 0x0B:
                    return "jmper";
                case 0x0C:
                    return "jmpes";
                case 0x0D:
                    return "jmpsr";
                case 0x0E:
                    return "jmpss";
                case 0x0F:
                    return "jmpwr";
                case 0x10:
                    return "jmpws";
                case 0x11:
                    return "jmpmf";
                case 0x12:
                    return "jmpmt";
                case 0x13:
                    return "jmpof";
                case 0x14:
                    return "jmpot";
                case 0x15:
                    return "jmprz";
                case 0x16:
                    return "jmprn";
                case 0x17:
                    return "mod";
                case 0x18:
                    return "move";
                case 0x19:
                    return "mult";
                case 0x1A:
                    return "or";
                case 0x1B:
                    return "rand";
                case 0x1C:
                    return "sub";
                default:
                    return "nop";
            }
            break;
    }
    return "nop";
}

char *getAddresses(instruction i){
    int x1, x2, y1, y2, addressing, addressing2;
    char indirect1, direct1, indirect2, direct2;
    char final[128];

    switch(i >> 63){
        case 0:
            y1 = i & 0x7ffffff;
            x1 = (i >> 27) & 0x3ffffff;
            addressing = (i >> 55) & 0xf;
            switch(addressing & 0x3){
                case 0:
                    indirect1 = '#';
                    break;
                case 1:
                    indirect1 = '^';
                    break;
                case 2:
                    indirect1 = '@';
                    break;
                case 3:
                    indirect1 = '&';
                    break;
            }
            switch(addressing >> 2){
                case 0:
                    direct1 = '#';
                    break;
                case 1:
                    direct1 = '^';
                    break;
                case 2:
                    direct1 = '&';
                    break;
                case 3:
                    direct1 = '#';
                    break;
            }
            sprintf(final,"%c(%c0x%07X,0x%07X)",indirect1,direct1,x1,y1);
            break;
        case 1:
            y2 = i & 0x1fff;
            x2 = (i >> 13) & 0x1fff;
            addressing2 = (i >> 26) & 0xf;
            y1 = (i >> 30) & 0xfff;
            x2 = (i >> 42) & 0xfff;
            addressing = (i >> 54) & 0xf;
            switch(addressing & 0x3){
                case 0:
                    indirect1 = '#';
                    break;
                case 1:
                    indirect1 = '^';
                    break;
                case 2:
                    indirect1 = '@';
                    break;
                case 3:
                    indirect1 = '&';
                    break;
            }
            switch(addressing >> 2){
                case 0:
                    direct1 = '#';
                    break;
                case 1:
                    direct1 = '^';
                    break;
                case 2:
                    direct1 = '&';
                    break;
                case 3:
                    direct1 = '#';
                    break;
            }
            switch(addressing2 & 0x3){
                case 0:
                    indirect2 = '#';
                    break;
                case 1:
                    indirect2 = '^';
                    break;
                case 2:
                    indirect2 = '@';
                    break;
                case 3:
                    indirect2 = '&';
                    break;
            }
            switch(addressing2 >> 2){
                case 0:
                    direct2 = '#';
                    break;
                case 1:
                    direct2 = '^';
                    break;
                case 2:
                    direct2 = '&';
                    break;
                case 3:
                    direct2 = '#';
                    break;
            }
            sprintf(final,"%c(%c0x%03X,0x%03X) %c(%c0x%04X,0x%04X)",indirect1,direct1,x1,y1,indirect2,direct2,x2,y2);
            break;
    }
    return final;
}

// Caller has to know whether to read the data out of the coordinates returned
// or if the coordinates are what its loooking for
data getData(int addressing, coordinate x, coordinate y){
    data currentData;
    data pointer = *pointers;
    cell tempCell;
    exploded_instruction inst;

    switch(addressing){
        case 0x0: // #(#X,Y)
        case 0xC:
            currentData.x = x;
            currentData.y = y;
            break;
        case 0x1: // ^(#X,Y)
        case 0xD:
            currentData.x = pointer.x + x;
            currentData.y = pointer.y + y;
            break;
        case 0x2: // @(#X,Y)
        case 0xE:
            currentData.x = pointer.x + x;
            currentData.y = pointer.y + y;
            break;
        case 0x3: // &(#X,Y)
        case 0xF:
            tempCell = grid[pointer.x][pointer.y];
            currentData.x = tempCell.owner.origin.x + x;
            currentData.y = tempCell.owner.origin.y + y;
            break;
        case 0x4: // #(^X,Y)
            currentData.x = pointer.x + x;
            currentData.y = pointer.y + y;
            break;
        case 0x5: // ^(^X,Y)
            tempCell = grid[pointer.x + x][pointer.y + y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + inst.x1;
                currentData.y = pointer.x + inst.y1;
            }
            break;
        case 0x6: // @(^X,Y)
            tempCell = grid[pointer.x + x][pointer.y + y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + x + inst.x1;
                currentData.y = pointer.x + y + inst.y1;
            }
            break;
        case 0x7: // &(^X,Y)
            tempCell = grid[pointer.x + x][pointer.y + y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = tempCell.owner.origin.x + inst.x1;
                currentData.y = tempCell.owner.origin.y + inst.y1;
            }
            break;
        case 0x8: // #(&X,Y)
            tempCell = grid[pointer.x][pointer.y];
            currentData.x = tempCell.owner.origin.x + x;
            currentData.y = tempCell.owner.origin.y + y;
            break;
        case 0x9: // ^(&X,Y)
            tempCell = grid[pointer.x][pointer.y];
            tempCell = grid[pointer.x + tempCell.owner.origin.x][pointer.y + tempCell.owner.origin.y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + inst.x1;
                currentData.y = pointer.x + inst.y1;
            }
            break;
        case 0xA: // @(&X,Y)
            tempCell = grid[pointer.x][pointer.y];
            tempCell = grid[pointer.x + tempCell.owner.origin.x][pointer.y + tempCell.owner.origin.y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + x + inst.x1;
                currentData.y = pointer.x + y + inst.y1;
            }
            break;
        case 0xB:  // &(&X,Y)
            tempCell = grid[pointer.x][pointer.y];
            tempCell = grid[pointer.x + tempCell.owner.origin.x][pointer.y + tempCell.owner.origin.y];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = tempCell.owner.origin.x + inst.x1;
                currentData.y = tempCell.owner.origin.y + inst.y1;
            }
            break;
    }

    return currentData;
}

int cellCmp(cell cell1, cell cell2){
    int match = 0;

    if(cell1.inst != cell2.inst){
        match = -1;
    }
    if(cell1.n != cell2.n){
        match = -1;
    }
    if(cell1.e != cell2.e){
        match = -1;
    }
    if(cell1.w != cell2.w){
        match = -1;
    }
    if(cell1.s != cell2.s){
        match = -1;
    }

    return match;
}

exploded_instruction explodeInstruction(instruction i){
    exploded_instruction retVal;
    if(i >> 63 == 0){
        retVal.arguments = 0;
        retVal.inst = (i >> 59) & 0xf;
        retVal.y1 = i & 0x7ffffff;
        retVal.x1 = (i >> 27) & 0x3ffffff;
        retVal.addressing1 = (i >> 55) & 0xf;
    } else {
        retVal.arguments = 1;
        retVal.inst = (i >> 58) & 0x1f;
        retVal.y2 = i & 0x1fff;
        retVal.x2 = (i >> 13) & 0x1fff;
        retVal.addressing2 = (i >> 26) & 0xf;
        retVal.y1 = (i >> 30) & 0xfff;
        retVal.x2 = (i >> 42) & 0xfff;
        retVal.addressing1 = (i >> 54) & 0xf;
    }

    return retVal;
}

void executeInstruction(data current_location){

}
