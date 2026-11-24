#include "../../include/level.h"
#include "../../include/util.h"
#include <fstream>
#include <iostream>
#include <fstream>

using namespace std;

bool filecheck(const char* filename) {
    ifstream file(filename);
    
    if (!file.is_open()) 
    {
        cout<< "Error. File cannot be opened" << endl;
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


// Modular implementation using primitive arrays for map parsing and verification
