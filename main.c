#include <stdio.h>
#include <time.h>
#include <math.h>
#include "execution.h"

int main(){
    data d;
    int i,j;

    fillGrid();

    pointers = malloc(sizeof(data));
    pointers_length = 1;
    for(i = GRID_SIZE; i >= 0; i--){
        for(j = GRID_SIZE; j >= 0; j--){
            if(cellCmp(grid[i][j],emptyCell) != 0 && (grid[i][j].n + grid[i][j].e + grid[i][j].w + grid[i][j].s) > 0){
                d.x = i;
                d.y = j;
            }
        }
    }
    pointers[0] = d;

    newPointers = NULL;
    newpointers_length = 0;
    follow_flow = 1;

    for(i = 0; i < 20; i++){
        printf("Tick %d:\n",i);
        tick();
    }

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
                grid[i][j].prot = rand() & 1;
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

void printCell(int i,int j){
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
    data pointer = pointers[0];
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
            tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
            currentData.x = tempCell.owner.origin.x + x;
            currentData.y = tempCell.owner.origin.y + y;
            break;
        case 0x4: // #(^X,Y)
            currentData.x = pointer.x + x;
            currentData.y = pointer.y + y;
            break;
        case 0x5: // ^(^X,Y)
            tempCell = grid[positive_modulo(pointer.x + x,GRID_SIZE)][positive_modulo(pointer.y + y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + inst.x1;
                currentData.y = pointer.x + inst.y1;
            }
            break;
        case 0x6: // @(^X,Y)
            tempCell = grid[positive_modulo(pointer.x + x,GRID_SIZE)][positive_modulo(pointer.y + y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + x + inst.x1;
                currentData.y = pointer.x + y + inst.y1;
            }
            break;
        case 0x7: // &(^X,Y)
            tempCell = grid[positive_modulo(pointer.x + x,GRID_SIZE)][positive_modulo(pointer.y + y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = tempCell.owner.origin.x + inst.x1;
                currentData.y = tempCell.owner.origin.y + inst.y1;
            }
            break;
        case 0x8: // #(&X,Y)
            tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
            currentData.x = tempCell.owner.origin.x + x;
            currentData.y = tempCell.owner.origin.y + y;
            break;
        case 0x9: // ^(&X,Y)
            tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
            tempCell = grid[positive_modulo(pointer.x + tempCell.owner.origin.x,GRID_SIZE)][positive_modulo(pointer.y + tempCell.owner.origin.y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + inst.x1;
                currentData.y = pointer.x + inst.y1;
            }
            break;
        case 0xA: // @(&X,Y)
            tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
            tempCell = grid[positive_modulo(pointer.x + tempCell.owner.origin.x,GRID_SIZE)][positive_modulo(pointer.y + tempCell.owner.origin.y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = pointer.x + x + inst.x1;
                currentData.y = pointer.x + y + inst.y1;
            }
            break;
        case 0xB:  // &(&X,Y)
        tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
        tempCell = grid[positive_modulo(pointer.x + tempCell.owner.origin.x,GRID_SIZE)][positive_modulo(pointer.y + tempCell.owner.origin.y,GRID_SIZE)];
            inst = explodeInstruction(tempCell.inst);
            if(inst.inst == 0x0){ // Only works if data!
                currentData.x = tempCell.owner.origin.x + inst.x1;
                currentData.y = tempCell.owner.origin.y + inst.y1;
            }
            break;
    }

    return currentData;
}

data getCellAddress(int addressing,coordinate x,coordinate y){
    data currentData;
    data pointer = pointers[0];
    cell tempCell;
    exploded_instruction inst;

    switch(addressing){
        case 0x0: // #(#X,Y)
        case 0xC:
        case 0x1: // ^(#X,Y)
        case 0xD:
        case 0x2: // @(#X,Y)
        case 0xE:
        case 0x4: // #(^X,Y)
        case 0x8: // #(&X,Y)
            currentData.x = pointer.x;
            currentData.y = pointer.y;
            break;
        case 0x3: // &(#X,Y)
        case 0xF:
            tempCell = grid[pointer.x % GRID_SIZE][pointer.y % GRID_SIZE];
            currentData.x = tempCell.owner.origin.x + x;
            currentData.y = tempCell.owner.origin.y + y;
            break;
        case 0x5: // ^(^X,Y)
        case 0x6: // @(^X,Y)
        case 0x7: // &(^X,Y)
            currentData.x = pointer.x + x;
            currentData.y = pointer.y + y;
            break;
        case 0x9: // ^(&X,Y)
        case 0xA: // @(&X,Y)
        case 0xB:  // &(&X,Y)
            currentData.x = pointer.x + tempCell.owner.origin.x;
            currentData.y = pointer.y + tempCell.owner.origin.y;
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

int dataCmp(data d1, data d2){
    int match = 0;

    if(d1.x != d2.x){
        match = -1;
    }

    if(d1.y != d2.y){
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
    exploded_instruction i;
    data arg1,arg2;
    data pointer = pointers[0];

    printCell(current_location.x,current_location.y);
    i = explodeInstruction(grid[current_location.x][current_location.y].inst);
    if(i.arguments == 0){
        arg1 = getData(i.addressing1,i.x1,i.y1);
        if(dataCmp(arg1,emptyData) == 0){
            return;
        }
        switch(i.inst){
            case 0:
                //return "data";
                return;
            case 1:
                jmpInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "jmp";
            case 2:
                notInst(arg1,getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "not";
            case 3:
                proInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "prot";
            case 4:
                rsnInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "rstn";
            case 5:
                rseInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "rste";
            case 6:
                rssInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "rsts";
            case 7:
                rswInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "rstw";
            case 8:
                stnInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "setn";
            case 9:
                steInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "sete";
            case 0xa:
                stsInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "sets";
            case 0xb:
                stwInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "setw";
            case 0xc:
                splInst(getCellAddress(i.addressing1,i.x1,i.y1));
                break;
                //return "split";
            default:
                //return "nop";
                return;
        }
    } else {
        arg1 = getData(i.addressing1,i.x1,i.y1);
        arg2 = getData(i.addressing2,i.x2,i.y2);
        if((dataCmp(arg1,emptyData) == 0) || (dataCmp(arg2,emptyData) == 0)){
            return;
        }
        switch(i.inst){
            case 0x00:
                addInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "add";
            case 0x01:
                andInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "and";
            case 0x02:
                divInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "div";
            case 0x03:
                jxzInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpxz";
            case 0x04:
                jxnInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpxn";
            case 0x05:
                jyzInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpyz";
            case 0x06:
                jynInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpyn";
            case 0x07:
                jbzInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpbz";
            case 0x08:
                jbnInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpbn";
            case 0x09:
                jnrInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpnr";
            case 0x0A:
                jnsInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpns";
            case 0x0B:
                jerInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmper";
            case 0x0C:
                jesInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpes";
            case 0x0D:
                jsrInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpsr";
            case 0x0E:
                jssInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpss";
            case 0x0F:
                jwrInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpwr";
            case 0x10:
                jwsInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpws";
            case 0x11:
                jmfInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpmf";
            case 0x12:
                jmtInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpmt";
            case 0x13:
                jofInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpof";
            case 0x14:
                jotInst(getCellAddress(i.addressing1,i.x1,i.y1),getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmpot";
            case 0x15:
                jrzInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmprz";
            case 0x16:
                jrnInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "jmprn";
            case 0x17:
                modInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "mod";
            case 0x18:
                moveInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "move";
            case 0x19:
                mulInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "mult";
            case 0x1A:
                orInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "or";
            case 0x1B:
                ranInst(arg1,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "rand";
            case 0x1C:
                subInst(arg1,arg2,getCellAddress(i.addressing2,i.x2,i.y2));
                break;
                //return "sub";
            default:
                return;
                //return "nop";
        }
    }
}

void saveData(data save_to, data d){
    if(grid[save_to.x][save_to.y].prot == 1){
        grid[save_to.x][save_to.y].prot = 0;
    } else {
        if(grid[save_to.x][save_to.y].inst >> 54 == 0){
            //grid[save_to.x][save_to.y].inst = (d.x << 27) | d.y;
            writes = realloc(writes,sizeof(write));
            writes[write_length].loc = save_to;
            writes[write_length].inst = (d.x << 27) | d.y;
            write_length++;
        }
    }
}
