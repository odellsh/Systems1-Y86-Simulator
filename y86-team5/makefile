CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Put object files in next line
OBJS = Memory.o ProgRegisters.o Sim.o Tools.o Y86.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o

# The OBJTEST macro is same as 'OBJS' above except replace Sim.o with testSim.o 
OBJTEST =  Memory.o ProgRegisters.o Sim.o Tools.o Y86.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o

#complete the following targets with appropriate dependencies and commands
yess:	$(OBJS) 
	$(CXX) $(OBJS) $(CXXFLAGS) -o yess

ytest:  $(OBJTEST) 
	$(CXX) $(OBJTEST) $(CXXFLAGS) -o ytest


#your object file targets below here...
Memory.o: Memory.h Tools.h

ProgRegisters.o: ProgRegisters.h Tools.h

Tools.o: Tools.h

Y86.o: Y86.h Memory.h ProgRegisters.h FetchStage.h DecodeStage.h ExecuteStage.h MemoryStage.h WritebackStage.h

Sim.o: Sim.h Y86.h Memory.h ProgRegisters.h 

##Y86Loader.o: Y86Loader.cpp Memory.h Tools.h

Y86trace.o: Sim.h Y86.h

FetchStage.o: Sim.h FetchStage.h Tools.h PipeStage.h Register.h DecodeStage.h ExecuteStage.h Memory.h

DecodeStage.o: Y86.h DecodeStage.h ProgRegisters.h PipeStage.h ExecuteStage.h Register.h

ExecuteStage.o: Sim.h ExecuteStage.h PipeStage.h Register.h DecodeStage.h MemoryStage.h Memory.h

MemoryStage.o: Sim.h MemoryStage.h Memory.h PipeStage.h ExecuteStage.h WritebackStage.h Register.h 

WritebackStage.o: Sim.h WritebackStage.h ProgRegisters.h PipeStage.h Register.h FetchStage.h  DecodeStage.h ExecuteStage.h MemoryStage.h

# clean target
# Remeber, you don't want to delete your testSim.o file
# The following target preserves testSim.o
clean:
		cp Y86Loader.o Y86Loader-tmp 
##		cp Memory.o Memory-tmp
		-rm *.o yess 
		cp Y86Loader-tmp Y86Loader.o
##		cp Memory-tmp Memory.o

