/*
    File:   Sim.cpp
    Desc:   The main program. Accepts and verifies parameters and executes simulator
            on the specified object file. Other parameters are used to specify the stage or
            stages where trace outputs will be printed. Trace outputs may include the 
            stage register contents, memory contents, and/or register contents. Trace outputs
            are printed at the end of a cycle.
            
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include "Sim.h"
#include "Y86.h"
#include "Memory.h"


using namespace std;



Y86 y86;  // Declare global Y86 object. 

void usage(void);
char *getOptions(int argc, char *argv[], bool *pf, bool *pd, bool *pe, bool *pm, bool *pw);

int main(int argc, char *argv[])
{	
	bool f=false,d=false,e=false,m=false,w=false;
	char *new_argv = getOptions(argc,argv,&f,&d,&e,&m,&w);

	y86.reset();

	if(y86.load(new_argv)){
	//cout << "return from load " << flag << std::endl;
    	y86.setTrace(f,d,e,m,w);  // this must come after the load, before entering while loop
    	bool stop = false;

    	while (!stop){
       		y86.clock();
       		y86.trace();
       		stop = y86.getStat();
		}
	}
    // After exiting simulation loop, always dump state
	y86.dumpProcessorRegisters();
	y86.dumpProgramRegisters();
	y86.dumpMemory();

    
} // end main

//
// getOptions - collects command line arguments for controlling trace output
// 
char * getOptions(int argc, char *argv[], bool *pf, bool *pd, bool *pe, bool *pm, bool *pw)
{
	char is;
	while((is = getopt(argc, argv, "fdemw")) != -1){
		if(is == 'f'){
			*pf = true;
		}
		else if(is  == 'd')
			*pd = true;
		else if(is == 'e')
			*pe = true;
		else if(is == 'm')
			*pm = true;
		else if(is == 'w')
			*pw = true;
		else
			is = false;
	}
	return argv[optind];
}

void usage(void)
{
    printf("Usage: yess [-fdemw] <objectfile>\n"
          "<objectfile> must be created with the Y86 assembler 'yas' (has .yo extension.)\n"
		  "[-fdemw] are optional flags that enable trace output for any or all stages.\n\n");
    exit(0);
}
