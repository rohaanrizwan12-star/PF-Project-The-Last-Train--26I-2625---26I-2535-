#include "physics.h"
#include "util.h"
# include <iostream>
// Movement validation and resource adjustments without structures

using namespace std;

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType)
{

    outConnType= '\0';
    
    if (fromF <1 || fromF > NUM_FLOORS || fromR < 1 || fromR > ROOMS_PER_FLOOR ||
        toF < 1 || toF > NUM_FLOORS || toR < 1 || toR > ROOMS_PER_FLOOR)
    {
        return false;
    }

    if (fromF == toF)
    {
        if (toR != fromR+1 && toR != fromR-1)
        {
            return false;
        }

    int leftroom= fromR<toR? fromR: toR;
    int indexfloor= fromF-1;
    char connection= horizontalConn[indexfloor][leftroom-1];


    if (connection == 'W' || connection == 'D' || connection == 'L' || connection == '1')
        {
            outConnType= connection;
            return true;
        }
    return false;

    }

    int lowerfloor=0;
    int uperfloor=0;

    if (fromF<toF)
    {
        lowerfloor= fromF;
        uperfloor= toF;
    }
    else
    {
        lowerfloor= toF;
        uperfloor= fromF;
    }

    if (fromR==1 && toR==1 && ((lowerfloor == 1 && uperfloor == 2) || 
    (lowerfloor == 2 && uperfloor == 3) || (lowerfloor == 4 && uperfloor == 5)))
    {   outConnType = 'S';
            return true;
    }
    
    if (lowerfloor == 1 && uperfloor == 2 && fromR == toR && (fromR == 2 || fromR == 3)) 
    {
        outConnType = '1';
        return true;
    }

    bool tolift= toR==4 && (toF==2 || toF==4 || toF==5);
    bool fromlift= fromR==4 && (fromF==2 || fromF==4 || fromF==5);

    if (tolift && fromlift)
    {
        outConnType= '2';
        return true;
    }
    return false;
}


bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{
    if (targetFloor < 1 || targetFloor > NUM_FLOORS || targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR)
    {
        return false;
    }

    switch (connType)
    {
        case 'D':
        case 'W':
        case 'S':
        case '1':
            return true;
        case 'L':
            return hasItem(inventory, inventoryCount, '<'); //Checks if the player has a key for locked doors
        case '2':
            if (lift2Operational)
            {

                return hasItem(inventory, inventoryCount, '('); //Check if the player has a fuse for lift 2
            }
            return emergencyTripsLeft > 0 && hasItem(inventory, inventoryCount, '$'); //Check if the player has an emergency card
        default:
            return false;
        
    }
}


void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)
{
    energy= clampInt(energy+deltaEnergy, 0, MAX_ENERGY);
    time= clampInt(time+deltaTime, 0, MAX_TIME);
}


void applyScoreDelta(int& score, int deltaScore)
{
    score += deltaScore;
}


bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
    {
        return false;
    }
    
    for(int i=0; i<inventoryCount; i++)
    {
        if (inventory[i]== itemSymbol)
            return true;
    }
    return false;
}


bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    if (inventoryCount < 0 || inventoryCount >= MAX_INVENTORY_CAPACITY)
    {
        return false;
    }

    inventory[inventoryCount]= itemSymbol;
    inventoryCount++;
    return true;
}


bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
    {
        return false;
    }

    for(int i=0; i<inventoryCount; i++)
    {
        if (inventory[i]== itemSymbol)
        {
            for(int j=i; j<inventoryCount-1; j++)
            {
                inventory[j]= inventory[j+1];
            }
            inventoryCount--;
            inventory[inventoryCount]= ENT_NONE;
            return true;
        }
    }
    return false;
}
