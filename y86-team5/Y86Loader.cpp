#include "Y86.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tools.h"
#include <string>
#include <ctype.h>

#define STARTADDR 2 //column where the hex address starts
#define ENDADDR3 4 //column where the hex address ends
#define ENDADDR4 5 //column where the hex addres ends if the address is 4 characters
#define DATSTART 7 //column where the data starts
#define DATEND 26 //column where the data ends
#define CSTART3 28 //column where the '|' is
#define CSTART4 29 //coulmn where the '|' is when the address is 4 characters
#define DATLENGTH 20 //maximum length of the data

//getAddrLength()
int getAddrLength(std::string line)
{
    if(line[5] == ':') return 3;
    if(line[6] == ':') return 4;
    return 0;
}
//isValidFileName
    //get the file name as a string, look to make sure the last
    //three characters are ".yo"
bool isValidFileName(std::string name)
{
    return(name.substr(name.length() - 3) == ".yo");   
}
//isValidAddress
    //returns true if the address is legal
bool isValidAddress(uint64_t addr)
{
    std::string s = std::to_string(addr);
    std::stringstream ss;
    ss << s;
    int addr_i;
    ss >> std::hex >> addr_i;
    return(addr > 0 && addr < 0x2000);     
}
//isCommentLine
    //looks at specific columns:
    // > column 5 is a colon (?)
    // > columns 6 through 27 are spaces
    // > columns 28 has a '|'
bool isCommentLine(std::string line){
    int i = 0;
    while(i < line.length()){
         if(line[i] == '#' || line[i] == '|') return true;
         else if (line[i] == ' ' || line[i] == '\n' | line[i] == '|') i++;
         else return false;
    }
}

//isBlankLine
    //returns true if the line is blank
bool isBlankLine(std::string line)
{
    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] != ' ') return false;
        return true;
    }
}
//hasData
    //looks to see if there is data in columns 8 through 26
bool hasData(std::string line)
{
    return(line[DATSTART] != ' ');
}
//hasValidData
    //returns zero if the data is bad, otherwise return the number
    //of bytes of data
int hasValidData(std::string line)
{   
    std::string data = line.substr(7, DATLENGTH + 1);
    int end = data.find_first_not_of("0123456789abcdefABCDEF",0);
    data = data.substr(0, end);
    if (data.find_first_not_of("0123456789abcedfABCEDF", 0) != std::string::npos)
        return 0;
    return data.length() / 2;
}

//getAddress
    //assuming the line is not blank or just a comment;
    //looks at the characters before the colon and converts to an address
uint64_t getAddress(std::string line)
{        
            std::string ss = "";
            ss += line.c_str()[2];
            ss += line.c_str()[3];
            ss += line.c_str()[4];
            if(getAddrLength(line) == 4) ss += line.c_str()[5];
            uint64_t ret = std::stoi(ss, 0, 16);
            return ret;
}
//hasSpaces
    //takes a record as input, and returns true if there are spaces begginig at the start
    //position through the end position
bool hasSpaces(std::string record, int start, int end)
{
    for(int i = start; i < end; i++)
    {
        if(record[i] == ' ') return true;
    }
    return false;
}
//checkHex
    //takes a record as input, and returns true if there are hex characters beggining at the 
    //start position through the end position
bool checkHex(std::string hex){
    for (unsigned int i = 0; i < hex.length(); i++){
        if(!isxdigit(hex[i])){
            return false;
        }
    }
    return true;
}

void Y86::storeByte(std::string two_hex, int address){
    uint8_t byte = std::stoul(two_hex, NULL, 16);
    memory.putByte(address, byte);

}

void Y86::storeData(std::string fileLine, int numBytes){
    std::string instr = fileLine.substr(7, numBytes);
    uint64_t addy = getAddress(fileLine);

    for(int i = 7; i < 7 + (numBytes * 2); i += 2){
        std::string byteStr = fileLine.substr(i, 2);
        storeByte(byteStr, addy);
        addy += 1;
    }

}
//readFile
    //first make sure there are no errors opening the file
    //then loop through it line by line using the helper 
    //functions to load into memory
bool Y86::readFile(std::ifstream& in){
    uint64_t lineCount = 0;
    std::string line;
    while(std::getline(in, line))
    {
        lineCount++;
        if(isBlankLine(line) || isCommentLine(line)) continue;
        
        if(hasData(line))
        {
            uint64_t addr = getAddress(line);
            if(!isValidAddress(addr))
            {
                std::cout << "Error on line " << lineCount << "\n" << line << "\n";
                continue;
            }

            int bytes = hasValidData(line);
            if(bytes == 0)
            {
                std::cout << "Error on line " << lineCount << "\n" << line << "\n";
                continue;
            }
                 
            storeData(addr, line, bytes);
        }
        else
        {
            std::cout << "Error on line " << lineCount << "\n" << line << "\n";
        }

    }
}
//main
bool Y86::load (char *filename)
{
    Y86::reset();
    std::string name(filename)
    
    if(!isValidFileName(name)) return false;

    std::ifstream inf (name);
    if(inf.fail())
    {
        std::cout << "File open failed: " << name << "\n" << "Usage: yess " << name << ".yo\n";
        return false;
    }

    return readFile(inf)
}