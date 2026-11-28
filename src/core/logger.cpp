#include "logger.h"
#include "util.h"
#include <fstream>
#include <cstring>
#include <iostream>

// File persistence for savegame.txt and game_summary.txt using primitive variables

using namespace std;

bool saveGameStateToFile(const char* filename, int playerFloor, int playerRoom, int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    if (filename == nullptr) 
        return false;
    

    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "Position " << playerFloor << " " << playerRoom << endl;
    file << "Energy " << energy << endl;
    file << "Time " << time << endl;
    file << "Score " << score << endl;
    file << "EmergencyTrips " << emergencyTripsLeft << endl;

    int clampedInventoryCount = clampInt(inventoryCount, 0, MAX_INVENTORY_CAPACITY);
    file << "Inventory " << clampedInventoryCount << endl;
    for (int i = 0; i < clampedInventoryCount; i++) {
        file << inventory[i] << " " << endl;
    }

    file << "Objectives " << endl;
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        if (objectives[i])
            file << "1 " << endl;
        else
            file << "0 " << endl;
    }

 //To save the room entities and their counts incase if the player wants to load the game state later

  for ( int f = 0; f < NUM_FLOORS; f++) {
     for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        file << "Room " << f << " " << r << endl;
        int clampedEntityCount = clampInt(entityCounts[f][r], 0, MAX_ENTITIES_PER_ROOM);
        file << "EntityCount " << clampedEntityCount << endl;

        for (int e = 0; e < clampedEntityCount; e++) {
            file << roomEntities[f][r][e] << endl;
        }
     }
  }

 file << "END" << endl;
 if (!file) {
     return false;
 }
 

 file.close();
 return true;
}






bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) 

{
    if (filename == nullptr) 
        return false;

    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }    

    char line[32];

    //Temporary variables to hold the values read from the file
 
    int TempFloor = 0;
    int TempRoom = 0;
    int TempEnergy = 0;
    int TempTime = 0;
    int TempScore = 0;
    int TempEmergencyTrips = 0;
    int TempInventoryCount = 0;
    char TempInventory[MAX_INVENTORY_CAPACITY];
    bool TempObjectives[NUM_OBJECTIVES];
    char TempRoomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int TempEntityCounts[NUM_FLOORS][ROOMS_PER_FLOOR];



    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
        TempInventory[i] = ENT_NONE;
    }


    for (int f = 0; f < NUM_FLOORS; f++) {
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            TempEntityCounts[f][r] = 0;
            for (int e = 0; e < MAX_ENTITIES_PER_ROOM; e++) {
                TempRoomEntities[f][r][e] = ENT_NONE;
            }
        }
    }

    // validate the file format


    if (!(file >> line >> TempFloor >> TempRoom) || strcmp(line, "Position") != 0) {
        return false;
    }

    if (TempFloor < 0 || TempFloor >= NUM_FLOORS || TempRoom < 0 || TempRoom >= ROOMS_PER_FLOOR) { 
        return false;
    }

    if (!(file >> line >> TempEnergy) || strcmp(line, "Energy") != 0) {
        return false;
    }                                                               

    if (TempEnergy < 0 || TempEnergy > MAX_ENERGY) { 
        return false;
    }

    if (!(file >> line >> TempTime) || strcmp(line, "Time") != 0) {
        return false;
    }
    if (TempTime < 0 || TempTime > MAX_TIME) { 
        return false;
    }

    if (!(file >> line >> TempScore) || strcmp(line, "Score") != 0) {
        return false;
    }
    if (TempScore < 0) { 
        return false;
    }
    if (!(file >> line >> TempEmergencyTrips) || strcmp(line, "EmergencyTrips") != 0) {
        return false;
    }
    if (TempEmergencyTrips < 0) { 
        return false;
    }   

    if (!(file >> line >> TempInventoryCount) || strcmp(line, "Inventory") != 0) {
        return false;
    }
    if (TempInventoryCount < 0 || TempInventoryCount > MAX_INVENTORY_CAPACITY) {
        return false;
    }

    for (int i = 0; i < TempInventoryCount; i++) {
        if (!(file >> TempInventory[i]) || TempInventory[i] == ENT_NONE || TempInventory[i]  <= 32 || TempInventory[i] >= 127) {
            return false;
        }
    }

 if (!(file >> line) || strcmp(line, "Objectives") != 0) {
        return false;
    }

    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        int objectiveValue = 0;
        if (!(file >> objectiveValue) || (objectiveValue != 0 && objectiveValue != 1)) {
            return false;
        }
        TempObjectives[i] = (objectiveValue == 1);
    }

    for (int f = 0; f < NUM_FLOORS; f++) {
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            int FileFloor = 0;
            int FileRoom = 0;
            if (!(file >> line >> FileFloor >> FileRoom) || strcmp(line, "Room") != 0) {
                return false;
            }
            if (FileFloor < 0 || FileFloor >= NUM_FLOORS || FileRoom < 0 || FileRoom >= ROOMS_PER_FLOOR) {
                return false;
            }

            if (!(file >> line >> TempEntityCounts[FileFloor][FileRoom]) || strcmp(line, "EntityCount") != 0) {
                return false;
            }
            if (TempEntityCounts[FileFloor][FileRoom] < 0 || TempEntityCounts[FileFloor][FileRoom] > MAX_ENTITIES_PER_ROOM) {
                return false;
            }

            for (int e = 0; e < TempEntityCounts[FileFloor][FileRoom]; e++) {
                if (!(file >> TempRoomEntities[FileFloor][FileRoom][e]) || TempRoomEntities[FileFloor][FileRoom][e] == ENT_NONE || TempRoomEntities[FileFloor][FileRoom][e] <= 32 || TempRoomEntities[FileFloor][FileRoom][e] >= 127) {
                    return false;
                }
            }
        }
    }

    if (!(file >> line) || strcmp(line, "END") != 0) {
        return false;
    }

    // If all validations pass, assign the temporary values to the actual variables
    playerFloor = TempFloor;
    playerRoom = TempRoom;
    energy = TempEnergy;
    time = TempTime;
    score = TempScore;
    emergencyTripsLeft = TempEmergencyTrips;
    inventoryCount = TempInventoryCount;
    
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++){
        inventory[i] = TempInventory[i];
    }

    for (int i = 0; i < NUM_OBJECTIVES; i++){
        objectives[i] = TempObjectives[i];
    }

    for (int f = 0; f < NUM_FLOORS; f++) {
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            entityCounts[f][r] = TempEntityCounts[f][r];
            for (int e = 0; e < MAX_ENTITIES_PER_ROOM; e++) {
                roomEntities[f][r][e] = TempRoomEntities[f][r][e];
            }
        }
    }

    file.close();
    return true;

}


void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
{
 if (filename == nullptr) 
        return;

    ofstream file(filename);
    if (!file.is_open()) {
        return;
    }


    
    const char* objectiveNames[NUM_OBJECTIVES] = {
    "Master Key",
    "Clue 4",
    "Clue 2",
    "Clue 7",
    "Cabinet Unlocked",
    "Train Key",
    "Fuse Collected",
    "Fuse Installed",
    "Lift Pass",
    "Second Lift Activated",
    "Passenger Rescued",
    "Cat Carrier",
    "Cat Secured",
    "Train Prepared",
    "Mission Won",
    "Mission Failed"
};

file << "=============================================" << endl;
file << "            GAME SUMMARY REPORT              " << endl;    
file << "=============================================" << endl;

if (victory) {
    file << "Congratulations! You have successfully completed the game!" << endl;
} else {
    file << "Game Over! Better luck next time!" << endl;
}

file << "Final Score: " << score << endl;
file << "Remaining Energy: " << remainingEnergy << endl;
file << "Remaining Time: " << remainingTime << endl;

file << "Objectives Completed: " << endl;
int CompletedObjectivesCount = 0;
for (int i = 0; i < NUM_OBJECTIVES; i++) {
    if (objectives[i]) {
        file << "[YES] ";
        CompletedObjectivesCount++;
    } else {
        file << "[NO] ";
    }

    file << objectiveNames[i] << endl;
}


file << "Session Audit: " << endl;
file << "Total Objectives Completed: " << CompletedObjectivesCount << " / " << NUM_OBJECTIVES << endl;
if (victory) {
    file << "Game Completed Successfully!" << endl;
} else if (remainingEnergy <= 0) {
    file << "Game Over: Energy Depleted!" << endl;
} else if (remainingTime <= 0) {
    file << "Game Over: Time Expired!" << endl;
} else {
    file << "Game Over before all Objectives were completed!" << endl;
}
file << "=============================================" << endl;
file << "=============================================" << endl;


if (!file) {
    cout << "Error: Failed while writing the summary report!" << endl;
    file.close();
    return; 
}

file.close();
}











            

