#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"

#include <iostream>
#include <string>

using namespace std;

// Modular simulation loop using primitive variables
const char stairsconection= 'S';
const char lift1conection= '1';
const char lift2conection= '2';
const int emergencytriplimit=3;


//checking for valid room
bool validroom(int floor, int room)
{
    if (floor<1 || floor>NUM_FLOORS || room<1 || room>ROOMS_PER_FLOOR)
    {
        return false;
    }
    return true;
}

//checking for entities count

bool validentitycount(int count)
{
    if (count<0 || count>MAX_ENTITIES_PER_ROOM)
    {
        return false;
    }
    return true;
}



bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational)
{
    if (targetFloor == currentFloor && targetRoom == currentRoom)
    {
        cout << "Error. Already in the specified room" << endl;
        return false;
    }

    if (!validroom(targetFloor, targetRoom))
    {
        cout << "Error. Invalid target room coordinates" << endl;
        return false;
    }

    char connType = ' ';

    if (!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom, horizontalConn, connType))
    {
        cout << "Error. Target room is not directly connected" << endl;
        return false;
    }

    if (!canTraverse(targetFloor, targetRoom, connType, inventory, inventoryCount, emergencyTripsLeft, lift2Operational))
    {
        cout << "Error. Cannot traverse to the target room with current resources" << endl;
        return false;
    }

    int energyy=0;
    int timee=0;

    switch (connType)
    {
        case 'D':
            energyy= COST_MOVE_ORDINARY_ENERGY;
            timee= COST_MOVE_ORDINARY_TIME;
            break;
        case 'W':
            energyy= COST_MOVE_WINDOW_ENERGY;
            timee= COST_MOVE_WINDOW_TIME;
            break;
        case '1':
            energyy= COST_MOVE_LIFT_ENERGY;
            timee= COST_MOVE_LIFT_TIME;
            break;
        case '2':
            energyy= COST_MOVE_LIFT_ENERGY;
            timee= COST_MOVE_LIFT_TIME;
            if (!lift2Operational)
                emergencyTripsLeft--;
            break;
        default:
            cout << "Error. Invalid connection type" << endl;
            return false;
    }

    bool emergency;
    if (connType == '2' && !lift2Operational)
    {
        emergency = true;
    }
    else
    {
        emergency = false;
    }

    if (emergency && emergencyTripsLeft < 0 || !hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD))
    {
        cout << "Error. Cannot use emergency lift without emergency card and no emergency trips left" << endl;
        return false;
    }

    currentFloor = targetFloor;
    currentRoom = targetRoom;
    applyResourceDelta(energy, time, energyy, timee);

    if (emergency)
    {
        emergencyTripsLeft--;
    }
    return true;
}

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
{
    
}