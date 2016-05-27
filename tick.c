#include "execution.h"

int compareWrites(const void *,const void *);

void tick(){
    int i, j;
    data p, new;

    writes = NULL;
    write_length = 0;

    while(j < pointers_length){
        p = pointers[0];
        executeInstruction(p);

        if(follow_flow == 1){
            i = grid[p.x][p.y].n + grid[p.x][p.y].e + grid[p.x][p.y].w + grid[p.x][p.y].s;
            newPointers = realloc(newPointers,i*sizeof(data));
            if(grid[p.x][p.y].n == 1){
                new.x = p.x;
                new.y = positive_modulo((p.y - 1),GRID_SIZE);
                newPointers[newpointers_length] = new;
                newpointers_length++;
            }

            if(grid[p.x][p.y].e == 1){
                new.x = (p.x + 1) % GRID_SIZE;
                new.y = p.y;
                newPointers[newpointers_length] = new;
                newpointers_length++;
            }

            if(grid[p.x][p.y].s == 1){
                new.x = p.x;
                new.y = (p.y + 1) % GRID_SIZE;
                newPointers[newpointers_length] = new;
                newpointers_length++;
            }

            if(grid[p.x][p.y].w == 1){
                new.x = positive_modulo((p.x - 1),GRID_SIZE);
                new.y = p.y;
                newPointers[newpointers_length] = new;
                newpointers_length++;
            }
        }

        pointers++;
        follow_flow = 1;
        j++;
    }

    qsort(writes,write_length,sizeof(write),compareWrites);

    for(i = 0; i < write_length; i++){
        if(i == write_length - 1){
            grid[writes[i].loc.x][writes[i].loc.y].inst = writes[i].inst;
        } else {
            if(writes[i].loc.x == writes[i+1].loc.x && writes[i].loc.y == writes[i+1].loc.y){
                if(rand() % 2 == 0){
                    grid[writes[i].loc.x][writes[i].loc.y].inst = writes[i].inst;
                } else {
                    grid[writes[i+1].loc.x][writes[i+1].loc.y].inst = writes[i+1].inst;
                }
                i++;
            } else {
                grid[writes[i].loc.x][writes[i].loc.y].inst = writes[i].inst;
            }
        }
    }

    memcpy(pointers,newPointers,sizeof(data) * newpointers_length);
    pointers_length = newpointers_length;

    free(newPointers);
    newPointers = NULL;
    newpointers_length = 0;
}

int compareWrites(const void *a,const void *b){
    const write *wa = (const write *)a;
    const write *wb = (const write *)b;

    return (wa->loc.x - wb->loc.x) + (wa->loc.y - wb->loc.y);
}

int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}
