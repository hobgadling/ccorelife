#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define instruction uint64_t
#define GRID_SIZE 10

typedef struct cell{
    instruction inst;
    unsigned char n;
    unsigned char e;
    unsigned char s;
    unsigned char w;
} cell;

cell grid[GRID_SIZE][GRID_SIZE];

char *getInstruction(instruction);
void fillGrid(void);
void printGrid(void);
char *getAddresses(instruction);

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
            num = rand();
            num = (num << 33) | rand();
            grid[i][j].inst = num;
            grid[i][j].n = rand() & 1;
            grid[i][j].e = rand() & 1;
            grid[i][j].w = rand() & 1;
            grid[i][j].s = rand() & 1;
        }
    }
}

void printGrid(){
    int i,j;

    for(i = 0; i < GRID_SIZE; i++){
        for(j = 0; j < GRID_SIZE; j++){
            printf("%ix%i: ",i,j);
            printf("%llX: ",grid[i][j].inst);
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
            sprintf(final,"%c(%c%X,%X)",indirect1,direct1,x1,y1);
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
            sprintf(final,"%c(%c%X,%X) %c(%c%X,%X)",indirect1,direct1,x1,y1,indirect2,direct2,x2,y2);
            break;
    }
    return final;
}
