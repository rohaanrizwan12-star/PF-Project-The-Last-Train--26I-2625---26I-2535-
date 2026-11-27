#include "viewer_iface.h"
#include "util.h"

// Terminal UI and dashboard rendering using primitive parameter

#include <iostream>
using namespace std;

void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                        int emergencyTripsLeft) {

cout << "Player's Floor:" << playerFloor << endl << "Player's Room:" << playerRoom << endl;
cout << "Player's Remaining Energy:" << energy << endl;
cout << "Time Remaining:" << time << endl;
cout << "Player's Current Score:" << score << endl;
cout << "Inventory:" << endl;

if (inventoryCount == 0) {
    cout << "Empty";    
}
else
{
    for (int i = 0; i < inventoryCount; i++) {
    cout << inventory[i] << endl;
}
}
cout << "Emergency Trips Left:" << emergencyTripsLeft << endl;
}

#include <iostream>
using namespace std;



void renderRoomDetails(int floor, int room, const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM], int entityCount) {
    cout << "Current Floor:" << floor << endl;
    cout << "Current Room:" << room << endl;
    cout << "Entities in the Room:" << entityCount << endl;

    if (entityCount == 0) {
        cout << "No entities in this room, it's empty." << endl;
    }
    else
    {
        cout << "Entities in this room:" << endl;
        for (int i=0; i < entityCount; i++) {
            cout << roomEntities[floor][room][i] << endl;
        }
    }


} 


void renderAsciiBuilding(int playerFloor, int playerRoom) {
    cout << "ASCII Representation of the Building:" << endl;

    for (int f = 0; f < NUM_FLOORS; f++) {
        cout << "Current Floor:" << f + 1 << endl;
        for ( int r = 0; r < ROOMS_PER_FLOOR; r++) {
            cout << "Room:" << r + 1 << endl;
            if ( f == playerFloor && r == playerRoom)
            {
                cout << "Player is here" << endl;
            }
            else
            {
                cout << "Player is not here" << endl;
            }
        }
    }

}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {

    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        cout << "Objective:" << i + 1 << endl;

        if (objectives[i]) {
            cout << "Completed" << endl;
        }
        else {
            cout << "Not Completed" << endl;
        }
    }
}

