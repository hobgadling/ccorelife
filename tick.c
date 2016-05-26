#include "execution.h"

void tick(){
    int i, j, newpointers_length = 0;
    data p, new;
    data *newPointers = NULL;

    while(pointers != NULL){
        p = pointers[0];
        executeInstruction(p);

        i = grid[p.x][p.y].n + grid[p.x][p.y].e + grid[p.x][p.y].w + grid[p.x][p.y].s;
        newPointers = realloc(newPointers,i*sizeof(data));
        if(grid[p.x][p.y].n == 1){
            new.x = p.x;
            new.y = p.y - 1;
            newPointers[newpointers_length] = new;
            newpointers_length++;
        }

        if(grid[p.x][p.y].e == 1){
            new.x = p.x + 1;
            new.y = p.y;
            newPointers[newpointers_length] = new;
            newpointers_length++;
        }

        if(grid[p.x][p.y].s == 1){
            new.x = p.x;
            new.y = p.y + 1;
            newPointers[newpointers_length] = new;
            newpointers_length++;
        }

        if(grid[p.x][p.y].w == 1){
            new.x = p.x - 1;
            new.y = p.y;
            newPointers[newpointers_length] = new;
            newpointers_length++;
        }

        pointers += sizeof(data);
    }

    pointers = newPointers;
    pointers_length = newpointers_length;
}
