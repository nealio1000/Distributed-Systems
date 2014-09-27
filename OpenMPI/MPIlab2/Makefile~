# compiler for the source files
CXX=mpic++
# flags to pass to the compiler when compiling files
# enables debugging and disables optimisation
CXXFLAGS=-g -O0
# the list of object files that will compose your application
# these are basically the list of your .cpp files except with
# .cpp changed to .o
OBJECTS=mpitest.o
# the target application that we are trying to build
TARGET=mpitest


# the suffixes that this make file understands
.SUFFIXES: .cpp .o


# the main target that will build the entire target
# note that the lines immediately after all: must start with
# exactly one tab stop
all: $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)
	
# rule to clean out all of our compiled files
clean:
		rm $(OBJECTS) $(TARGET)
		
# rule to convert a .cpp file into a .ofile
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $*.cpp
		

