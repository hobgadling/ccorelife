#include "execution.h"

void addInst(data arg1, data arg2, data save_to){
    arg1.x = arg1.x + arg2.x;
    arg1.y = arg1.y + arg2.y;
    saveData(save_to,arg1);
}
