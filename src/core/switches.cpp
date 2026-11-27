#include "switches.h"
#include "util.h"
#include "state.h"

// Door lock mechanics, lifts, and cabinet verification without structure
#include <iostream>
using namespace std;


bool canUseLift1(int fromFloor, int toFloor)
{
    if (fromFloor == 1 && toFloor == 2)
    {
        return true;
    }
    else if (fromFloor == 2 && toFloor == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}



bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft){
    bool validFromFloor = (fromFloor == 2 || fromFloor == 4 || fromFloor == 5);
    bool validToFloor = (toFloor == 2 || toFloor == 4 || toFloor == 5);

    if (!validFromFloor || !validToFloor)
    {
        return false;
    }
    if (fromFloor == toFloor)
    {
        return false;
    }

    if (lift2Operational || emergencyTripsLeft > 0)
    {
        return true;
    }

    return false;
}


bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount){
    for (int i = 0; i < inventoryCount && i < MAX_INVENTORY_CAPACITY; i++)
    {
        if (inventory[i] == ENT_MASTER_KEY)
        {
            return true;
        }
    }
    return false;
}




bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, bool objectives[NUM_OBJECTIVES], int &score){

    if (objectives[OBJ_FUSE_INSTALLED])
    {
        return false; // fuse is already installed
    }

    int fuseIndex = -1;
    // Find the index of fuse
    for (int i = 0; i < inventoryCount && i < MAX_INVENTORY_CAPACITY; i++)
    {
        if (inventory[i] == ENT_FUSE)
        {
            fuseIndex = i;
            break;
        }
    }
    if (fuseIndex == -1)
    {
        return false; // Fuse not found
    }

    // Removing the fuse
    for (int i = fuseIndex; i < inventoryCount - 1; i++)
    {
        inventory[i] = inventory[i + 1];
    }
    inventoryCount--;

    objectives[OBJ_FUSE_INSTALLED] = true;
    score += SCORE_INSTALL_FUSE;

    return true;
}


bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]) {

    if (enteredCode == nullptr)
    {
        return false;
    }

    if (!objectives[OBJ_CLUE_4] || !objectives[OBJ_CLUE_2] || !objectives[OBJ_CLUE_7])
    {
        return false;
    }

    if (enteredCode[0] == ENT_CLUE_4 && enteredCode[1] == ENT_CLUE_2 && enteredCode[2] == ENT_CLUE_7 && enteredCode[3] == '\0') {
        return true;
    }
return false;


}









