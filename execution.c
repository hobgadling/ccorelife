#include "execution.h"

void addInst(data arg1, data arg2, data save_to){
    arg1.x = arg1.x + arg2.x;
    arg1.y = arg1.y + arg2.y;
    saveData(save_to,arg1);
}

void andInst(data arg1, data arg2, data save_to){
    arg1.x = arg1.x & arg2.x;
    arg1.y = arg1.y & arg2.y;
    saveData(save_to,arg1);
}

void divInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x / arg1.x;
    arg1.y = arg2.y / arg1.y;
    saveData(save_to,arg1);
}

void modInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x % arg1.x;
    arg1.y = arg2.y % arg1.y;
    saveData(save_to,arg1);
}

void moveInst(data arg1, data save_to){
    grid[save_to.x][save_to.y].inst = grid[arg1.x][arg1.y].inst;
}

void mulInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x * arg1.x;
    arg1.y = arg2.y * arg1.y;
    saveData(save_to,arg1);
}

void notInst(data arg1, data save_to){
    arg1.x = ~arg1.x;
    arg1.y = ~arg1.y;
    saveData(save_to,arg1);
}

void orInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x | arg1.x;
    arg1.y = arg2.y | arg1.y;
    saveData(save_to,arg1);
}

void proInst(data save_to){
    grid[save_to.x][save_to.y].prot = 1;
}

void ranInst(data arg1, data save_to){
    arg1.x = rand() % arg1.x;
    arg1.y = rand() % arg1.y;
    saveData(save_to,arg1);
}

void rseInst(data save_to){
    grid[save_to.x][save_to.y].e = 0;
}

void rsnInst(data save_to){
    grid[save_to.x][save_to.y].n = 0;
}

void rssInst(data save_to){
    grid[save_to.x][save_to.y].s = 0;
}

void rswInst(data save_to){
    grid[save_to.x][save_to.y].w = 0;
}

void subInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x - arg1.x;
    arg1.y = arg2.y - arg1.y;
    saveData(save_to,arg1);
}

void steInst(data save_to){
    grid[save_to.x][save_to.y].e = 1;
}

void stnInst(data save_to){
    grid[save_to.x][save_to.y].n = 1;
}

void stsInst(data save_to){
    grid[save_to.x][save_to.y].s = 1;
}

void stwInst(data save_to){
    grid[save_to.x][save_to.y].w = 1;
}
