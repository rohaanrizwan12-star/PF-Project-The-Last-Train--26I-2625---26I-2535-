#include "../../include/level.h"
#include "../../include/util.h"
#include <fstream>
#include <iostream>
#include <string>

// Modular implementation using primitive arrays for map parsing and verification

using namespace std;

bool filecheck(const char *filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error. " << filename << " cannot be opened" << endl;
        return false;
    }

    string line;
    bool data = false;

    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ' && line[i] != '\t')
            {
                data = true;
                break;
            }
        }
        if (data)
            break;
    }
    file.close();

    if (!data)
    {
        cout << "Error. File is empty" << endl;
        return false;
    }
    return true;
}

const char *getEntityName(char entitySymbol)
{
    switch (entitySymbol)
    {
    case ENT_MASTER_KEY:
        return "Master Key";
    case ENT_ENERGY_PACK:
        return "Energy Pack";
    case ENT_EMERGENCY_TOOLKIT:
        return "Emergency Toolkit";
    case ENT_CLUE_4:
        return "Clue Digit 4";
    case ENT_FUSE:
        return "Fuse";
    case ENT_CAT:
        return "Stranded Cat";
    case ENT_CONTROL_PANEL:
        return "Control Panel Terminal";
    case ENT_LIFT_PASS:
        return "Lift Pass";
    case ENT_PASSENGER:
        return "Stranded Passenger";
    case ENT_MEDICAL_KIT:
        return "Medical Kit";
    case ENT_EMERGENCY_CARD:
        return "Emergency Access Card";
    case ENT_CLUE_2:
        return "Clue Digit 2";
    case ENT_CLUE_7:
        return "Clue Digit 7";
    case ENT_CABINET_TRAIN_KEY:
        return "Cabinet / Train Key";
    case ENT_CAT_CARRIER:
        return "Cat Carrier";
    case ENT_OBSERVATION_TOKEN:
        return "Observation Token";
    default:
        return "Unknown Entity";
    }
}

bool loadBuildingMap(const char *filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    if (!filecheck(filename))
        return false;

    ifstream file(filename);
     if (!file)
        return false;

    // Initializing the starting point of each room

    const int roomstart[ROOMS_PER_FLOOR] = {9, 21, 33, 45};
    const int roomwidth = 11;
    const int lastwall = 56;

    char borderline[MAX_LINE_LEN];
    
    if (!file.getline(borderline, MAX_LINE_LEN))
        return false;

    for (int floor = NUM_FLOORS - 1; floor >= 0; floor--)
    {
        char layoutline[MAX_LINE_LEN] = {};
        char itemline[MAX_LINE_LEN] = {};

        if (!file.getline(borderline, MAX_LINE_LEN) || !file.getline(layoutline, MAX_LINE_LEN) || !file.getline(itemline, MAX_LINE_LEN))
        {
            cout << "Error reading file" << endl;
            return false;
        }
        if (layoutline[lastwall] != '|' || itemline[lastwall] != '|')
        {
            cout << "Error: Invalid line format" << endl;
            return false;
        }

        // Read the three gaps between the four rooms.

        for (int gap = 0; gap < ROOMS_PER_FLOOR - 1; ++gap)
        {
            int column = roomstart[gap] + roomwidth;
            char connection = layoutline[column];

            if (layoutline[column - 1] == 'L' && connection == '1')

                horizontalConn[floor][gap] = CONN_NONE;

            else if (connection == CONN_DOOR || connection == CONN_WINDOW || connection == CONN_LOCKED_DOOR || connection == CONN_SOLID_WALL)

                horizontalConn[floor][gap] = connection;

            else
            {
                cout << "Error. Invalid connection symbol" << endl;
                return false;
            }
        }
    for (int room = 0; room < ROOMS_PER_FLOOR; ++room)
    {
        int count = 0;
        for (int slot = 0; slot < MAX_ENTITIES_PER_ROOM; ++slot)
        {
            roomEntities[floor][room][slot] = ENT_NONE;
        }

        int start = roomstart[room];
        int end = start + roomwidth;

        for (int column = start; column < end; column++)
        {
            if (itemline[column] != ' ')
            {    
                if (count == MAX_ENTITIES_PER_ROOM)
                    return false;
            
            roomEntities[floor][room][count] = itemline[column];
            count++;

            }

        }
        entityCounts[floor][room]=count;
    }
}


return true;
}



bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{
    if (!filecheck(filename))
        return false;

    ifstream file(filename);
    if (!file)
        return false;

    string setting;
    string unusedval;

    int startingEnergy=0;
    int startingTime=0;
    int startingScore=0;

    bool foundenergy= false;
    bool foundtime= false;
    bool foundscore= false;

    while (file >> setting)
    {
        if (setting =="INITIAL_ENERGY:")
        {
                if (!(file>> startingEnergy))
                return false;

                foundenergy= true;
        }
        else if (setting=="INITIAL_TIME:")
        {
                if (!(file>> startingTime))
                return false;

                foundtime= true;
        }
        else if (setting=="INITIAL_SCORE:")
        {
                if (!(file>> startingScore))
                return false;

                foundscore= true;
        }
        else 
        {
            if (!(file>> unusedval))
                return false;
        }
    }
    if (!foundenergy || !foundtime || !foundscore)
    {
        cout << "Error. Missing initial state settings" << endl;
        return false;
    }
    energy= startingEnergy;
    time= startingTime;
    score= startingScore;

    inventoryCount=0;
    for(int i=0; i<MAX_INVENTORY_CAPACITY; i++)
    {
        inventory[i]= ENT_NONE;
    }
    for(int j=0; j<NUM_OBJECTIVES; j++)
    {
        objectives[j]= false;
    }

    emergencyTripsLeft=0;
    return true;
}


bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{
if (startFloor<1 || startFloor>NUM_FLOORS || startRoom<1 || startRoom>ROOMS_PER_FLOOR)
{
    cout << "Error. Invalid starting room coordinates" << endl;
    return false;
}

int indexfloor= startFloor-1;

//if there is a room on left
if(startRoom>1)
{
    char leftconn= horizontalConn[indexfloor][startRoom-2];

    if(leftconn==CONN_DOOR || leftconn==CONN_WINDOW)
    {
        return true;
    }
}

// if there's a room on right

if(startRoom<ROOMS_PER_FLOOR)
{
    char rightconn= horizontalConn[indexfloor][startRoom-1];

    if(rightconn==CONN_DOOR || rightconn==CONN_WINDOW)
    {
        return true;
    }
}

//on everyfloor room 1 has a way out
if(startRoom==1)
{
    return true;
}

// there is lift from room 2 to room 3 on floor 1 and floor 2

if ((startFloor==1 || startFloor==2) && (startRoom==2 || startRoom==3))
{
    
    return true;    
}
return false;
}
