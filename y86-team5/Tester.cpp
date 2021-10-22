#include <string>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "Memory.h"
#include "Y86.h"

using namespace std;

static bool isValidFileName(std::string fileName);
static void readFile(std::string fileName);
static uint64_t getAddress(std::string fileLine);
static bool hasValidAddress(std::string fileLine);
static bool checkHex(std::string line, int start, int end);
static bool hasData(std::string fileLine);
static int hasValidData(std::string fileLine);
static bool checkLine(std::string line);
static bool hasSpaces(std::string line, int start, int end);
static bool isCommentLine(std::string fileLine);


bool Y86::load(char *filename){
    readFile(std::string(filename));
    return false;
}


static bool isValidFileName(std::string fileName)
{
    if(fileName.find(".yo") != std::string::npos){ //if ".yo" is found in the filename
        return true;         //return true
    }
    else{
        return false;        //else return false;
    }
}

static void readFile(std::string fileName)
{
    std::ifstream inf;
    uint64_t address = 0;
    if(isValidFileName(fileName)){
        std::string str(fileName);

        inf.open(fileName.c_str());
        std::string readLine;
        int numLines = 1;
        int byteNum = 0;

        std::getline(inf, readLine);

        while(inf.good()){
            //cout << readLine << "\n";
            if(!checkLine(readLine)){
                cout << "Error on line " << numLines << "\n";
                cout << readLine << "\n";
                return;
            }
            std::getline(inf, readLine);
            numLines++;
        }
    }
    else{
        int val = fileName.length() - 3;
        std::string correctFile = fileName.substr(0, val);
        cout << "File open failed: " << fileName << "\n";
        cout << "Usage: yess <filename>.yo" << "\n"; 
    }
}

static bool checkLine(std::string line){

    bool has3DigAddy = false;
    bool hasValidInstr = false;
    bool hasValidAddy = true;

    std::string instr = line.substr(7, 20);
    std::string addy = line.substr(0, 7);
    int numBytes;

    static bool isFirstLine = true;
    static int previousAddy = 0;
    int currAddy;

    //bool checkingHex = checkHex(line, 7, 20);
    //cout << checkingHex << endl;

    if(addy[0] == '0' && addy[1] == 'x'){
        for(unsigned int i = 2; i < addy.length(); i++){
            if(addy[i] == ':' && i == 5 && addy[i + 1] == ' '){
                std::string appendedAddy = addy.substr(2,3);
                currAddy = std::stoul(appendedAddy, NULL, 16);

                has3DigAddy = true;
                i++;
            }
            else if(addy[i] == ':' && i == 6){
                std::string appendedAddy = addy.substr(2,4);
                currAddy = std::stoul(appendedAddy, NULL, 16);

                has3DigAddy = false;
            }
            else if(!isxdigit(addy[i]))
                hasValidAddy = false;
        }
    }
    if(hasData(line)){
        //cout << "has data" << endl;

        numBytes = hasValidData(line);
        //cout << numBytes << endl;
        if(numBytes){

            //opcode checking
            if(numBytes == 8){
                hasValidInstr = true;
            }
            //cout << "has valid data" << endl;
            else if(numBytes >= 0 && numBytes <= 10){
                char opcode = line[7];
                if(opcode == '2' || opcode == '6' || opcode == 'a' || opcode == 'A' || opcode == 'b' || opcode == 'B'){
                    if(numBytes == 2){
                        hasValidInstr = true;
                    }
                }
                else if(opcode == '0' || opcode == '1' || opcode == '9'){
                    if(numBytes == 1){
                        hasValidInstr = true;
                    }
                }
                else if(opcode == '7' || opcode == '8'){
                    if(numBytes == 9){
                        hasValidInstr = true;
                    }
                }
                else if(numBytes == 10){
                    hasValidInstr = true;
                }
            }

            if(isFirstLine){
                isFirstLine = false;
                previousAddy = numBytes;
            }
            else{
                //cout << currAddy << " " << previousAddy << endl;
                if(currAddy < previousAddy){
                    hasValidInstr = false;
                }
                previousAddy = currAddy + numBytes;
            }
        }
    }
    else{
        if(hasSpaces(instr, 0, instr.length())){
            hasValidInstr = true;
        }
    }

    bool hasValidComment = false;
    bool isComment = isCommentLine(line);

    if(has3DigAddy){
        if(line[28] == '|' && line[27] == ' ' && line[6] == ' '){
            hasValidComment = true;
        }
    }
    else{
        if(line[28] == ' ' && line[27] == ' ' && line[29] == '|'){
            hasValidComment = true;
        }
    }
    
    //cout << hasValidComment << hasValidInstr << hasValidAddy << "\n";
    return (hasValidComment && hasValidInstr && hasValidAddy) || isComment;
}

static bool hasValidAddress(std::string fileLine)
{
    if(fileLine.substr(0,2) == "0x"){
        if(fileLine.substr(5,2) == ": "){
            return checkHex(fileLine, 2, 4);
        }
        else if(fileLine.substr(6,1) == ":"){
            return checkHex(fileLine, 2, 5);
        }

    }
    else{
        return false;
    }
}

//WORKS
static bool isCommentLine(std::string fileLine)
{
    for(int i = 0; i < 28; i++){
        if(fileLine[i] != ' ') //if not blank or '|'
            return false;  //return false
    }
    if(fileLine[28] == '|')
        return true;                //else return true
    else
        return false;
}

//WORKS
static bool isBlankLine(std::string fileLine)
{
    for(unsigned int i = 0; i < fileLine.length(); i++){
        if(fileLine[i] != ' ')    //if something is there
            return false;
    }
    return true;
}

static bool hasData(std::string fileLine){
    if(fileLine[7] != ' ')
        return true;
    else
        return false;
}

static int hasValidData(std::string fileLine){
    int i = 7; //starting point
    int start = i;
    int end; //end variable
    char c = fileLine[i];

    while (c != ' '){
        //cout << c << " " << i << endl;
        i++;
        c = fileLine[i];
    }
    end = i - 1;
    if(!checkHex(fileLine, start, end)){
        //cout << "invalid data" << endl;
        return 0;
    }
    else{
        //cout << "valid data" << endl;
        return ceil((double)(end - start) / 2);
    }
}

static uint64_t getAddress(std::string fileLine){
    std::string addy;
    addy.append(fileLine, 2,3);
    uint64_t store = std::stoul(addy, NULL, 16); //turns into int
    return store;
}

static bool hasSpaces(std::string line, int start, int end){
    std::string sub = line.substr(start, (end - start) + 1);
    for(unsigned int i = 0; i < sub.length(); i ++){
        if(!isspace(sub[i])){
            return false;
        }
    }
    return true;
}

static bool checkHex(std::string line, int start, int end)
{
    //cout << line << " " << start << " " << end << endl;
    bool flag = true;
    for(int i = start; i <= end; i++){
        char check = line[i];
        if(check == '0' || check == '1' || check == '2' || check == '3' || check == '4' 
        || check == '5' || check == '6' || check == '7'|| check == '8' || check == '9'
        || check == 'a' || check == 'A'
        || check == 'b' || check == 'B'
        || check == 'c' || check == 'C'
        || check == 'd' || check == 'D'
        || check == 'e' || check == 'E'
        || check == 'f' || check == 'F')
        {
            continue;
        }
        else{
            flag = false;
        }
    }
    return flag;
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


int main(){
    //readFile(std::string("asumr.yo"));
    readFile(std::string("asumr.yi"));
}