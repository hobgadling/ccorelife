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

void jmpInst(data save_to){
    newPointers = realloc(newPointers,sizeof(data));
    newPointers[newpointers_length] = save_to;
    newpointers_length++;
    follow_flow = 0;
}

void jbnInst(data arg1, data save_to){
    if(arg1.x != 0 && arg1.y != 0){
        jmpInst(save_to);
    }
}

void jbzInst(data arg1, data save_to){
    if(arg1.x == 0 && arg1.y == 0){
        jmpInst(save_to);
    }
}

void jerInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].e == 0){
        jmpInst(save_to);
    }
}

void jesInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].e == 1){
        jmpInst(save_to);
    }
}

void jmfInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].owner.id != grid[pointers[0].x % GRID_SIZE][pointers[0].y % GRID_SIZE].owner.id){
        jmpInst(save_to);
    }
}

void jmtInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].owner.id == grid[pointers[0].x % GRID_SIZE][pointers[0].y % GRID_SIZE].owner.id){
        jmpInst(save_to);
    }
}

void jnrInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].n == 0){
        jmpInst(save_to);
    }
}

void jnsInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].n == 1){
        jmpInst(save_to);
    }
}

void jofInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].owner.id == grid[pointers[0].x % GRID_SIZE][pointers[0].y % GRID_SIZE].owner.id || cellCmp(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE],emptyCell) == 0){
        jmpInst(save_to);
    }
}

void jotInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].owner.id != grid[pointers[0].x % GRID_SIZE][pointers[0].y % GRID_SIZE].owner.id && cellCmp(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE],emptyCell) != 0){
        jmpInst(save_to);
    }
}

void jrnInst(data arg1, data save_to){
    if(rand() % arg1.x != 0 && rand() % arg1.y != 0){
        jmpInst(save_to);
    }
}

void jrzInst(data arg1, data save_to){
    if(rand() % arg1.x == 0 && rand() % arg1.y == 0){
        jmpInst(save_to);
    }
}

void jsrInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].s == 0){
        jmpInst(save_to);
    }
}

void jssInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].s == 1){
        jmpInst(save_to);
    }
}

void jwrInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].w == 0){
        jmpInst(save_to);
    }
}

void jwsInst(data arg1, data save_to){
    if(grid[arg1.x % GRID_SIZE][arg1.y % GRID_SIZE].w == 1){
        jmpInst(save_to);
    }
}

void jxnInst(data arg1, data save_to){
    if(arg1.x != 0){
        jmpInst(save_to);
    }
}

void jxzInst(data arg1, data save_to){
    if(arg1.x == 0){
        jmpInst(save_to);
    }
}

void jynInst(data arg1, data save_to){
    if(arg1.y != 0){
        jmpInst(save_to);
    }
}

void jyzInst(data arg1, data save_to){
    if(arg1.y == 0){
        jmpInst(save_to);
    }
}

void modInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x % arg1.x;
    arg1.y = arg2.y % arg1.y;
    saveData(save_to,arg1);
}

void moveInst(data arg1, data save_to){
    //grid[save_to.x][save_to.y].inst = grid[arg1.x][arg1.y].inst;
    writes = realloc(writes,sizeof(write));
    writes[write_length].loc = save_to;
    writes[write_length].inst = grid[arg1.x][arg1.y].inst;
    write_length++;
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
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].prot = 1;
}

void ranInst(data arg1, data save_to){
    arg1.x = rand() % arg1.x;
    arg1.y = rand() % arg1.y;
    saveData(save_to,arg1);
}

void rseInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].e = 0;
}

void rsnInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].n = 0;
}

void rssInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].s = 0;
}

void rswInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].w = 0;
}

void splInst(data save_to){
    newPointers = realloc(newPointers,sizeof(data));
    newPointers[newpointers_length] = save_to;
    newpointers_length++;
}

void steInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].e = 1;
}

void stnInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].n = 1;
}

void stsInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].s = 1;
}

void stwInst(data save_to){
    grid[save_to.x % GRID_SIZE][save_to.y % GRID_SIZE].w = 1;
}

void subInst(data arg1, data arg2, data save_to){
    arg1.x = arg2.x - arg1.x;
    arg1.y = arg2.y - arg1.y;
    saveData(save_to,arg1);
}
