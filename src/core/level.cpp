#include "../../include/level.h"
#include "../../include/util.h"
#include <fstream>
#include <iostream>
#include <fstream>

// Modular implementation using primitive arrays for map parsing and verification


using namespace std;

bool filecheck(const char* filename) 
{
    ifstream file(filename);
    
    if (!file.is_open()) 
    {
        cout<< "Error. "<<filename<<" cannot be opened" << endl;
        return false;
    }

    string line;
    bool data= false;

    while (getline(file, line)) 
    {
        for (int i=0; i<line.length(); i++)
        {
            if (line[i] != ' ' && line[i] != '\t') 
            {
                data= true;
                break;
            }
        }
        if (data)
        break;
    }
    file.close();

    if (!data) 
    {
        cout<< "Error. File is empty" << endl;
        return false;
    }
return true;
}

const char* getEntityName(char entitySymbol) 
{
    switch (entitySymbol) 
    {
        case ENT_MASTER_KEY: return "Master Key";
        case ENT_ENERGY_PACK: return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT: return "Emergency Toolkit";
        case ENT_CLUE_4: return "Clue Digit 4";
        case ENT_FUSE: return "Fuse";
        case ENT_CAT: return "Stranded Cat";
        case ENT_CONTROL_PANEL: return "Control Panel Terminal";
        case ENT_LIFT_PASS: return "Lift Pass";
        case ENT_PASSENGER: return "Stranded Passenger";
        case ENT_MEDICAL_KIT: return "Medical Kit";
        case ENT_EMERGENCY_CARD: return "Emergency Access Card";
        case ENT_CLUE_2: return "Clue Digit 2";
        case ENT_CLUE_7: return "Clue Digit 7";
        case ENT_CABINET_TRAIN_KEY: return "Cabinet / Train Key";
        case ENT_CAT_CARRIER: return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN: return "Observation Token";
        default: return "Unknown Entity";
    }
}

bool loadbuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) 
{
    if (!filecheck(filename))
        return false;

    ifstream file(filename);
        return false;

//clearing the array before using the map data

    for (int floor=0; floor<NUM_FLOORS; floor++) 
    {
        for (int gap=0; gap< ROOMS_PER_FLOOR - 1; ++gap) 
            horizontalConn[floor][gap] = CONN_NONE;
        
        for (int room=0; room<ROOMS_PER_FLOOR; ++room) 
        {
            entityCounts[floor][room] = 0;
            
            for (int item=0; item<MAX_ENTITIES_PER_ROOM; ++item) 
                roomEntities[floor][room][item] = ENT_NONE;
            
        }
    }

const int roomstart[ROOMS_PER_FLOOR]={9,21,33,45};
const int lastRoomStart=roomstart[ROOMS_PER_FLOOR-1];

bool foundfloor[NUM_FLOORS]={};
string line;

while (getline(file, line))
{
    if (line.length() < static_cast <size_t >(lastRoomStart + 5))
        continue;

    int firstroomstart= roomstart[0];
    char floordigit= line[firstroomstart - 1];
    int floor=floordigit - '0';

    if (floor<1 || floor>NUM_FLOORS)
    continue;

    //To check each label
    bool isflowrow= true;

    for(int room=0; room<ROOMS_PER_FLOOR; ++room)
    {
        int start= roomstart[room];
        string label= line.substr(start, 5);

        if (line[start] != 'F' || line[start + 1] != floordigit || line[start + 2] != '-' || line[start + 3] != 'R' || line[start + 4] != '1' + room) 
        {
            isflowrow= false;
            break;
        }
    }

    if (!isflowrow)
        continue;

    int floorIndex= floor - 1;

    if (foundfloor[floorIndex]) 
    {
        cout << "Error. Duplicate floor data for Floor " << floor << endl;
        return false;
    }

    //Read the three gaps between the four rooms.

    for (int gap=0; gap<ROOMS_PER_FLOOR - 1; ++gap) 
    {
        int column= roomstart[gap] + 11;
        char connection= line[column];

        if (line[column -1]=='L'&& connection == ' 1')
        {
            horizontalConn[floorIndex][gap] = CONN_LOCKED_DOOR;
        }
        else if (connection== CONN_DOOR || connection== CONN_WINDOW || connection== CONN_SOLID_WALL || connection== CONN_SOLID_WALL)
        {
            horizontalConn[floorIndex][gap] = connection;
        }
        else
        {
            cout << "Error. Invalid connection symbol"<<endl;
            return false;
        }

    }

    //The line imediately below the labels contains the entities.

    string entityLine;
    if (!getline(file, entityLine))
        return false;
    
    



}