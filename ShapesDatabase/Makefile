SRCS = Parser.cpp Circle.cpp Rectangle.cpp Triangle.cpp 
INCS = Shape.h Circle.h Rectangle.h Triangle.h ShapesDB.h
OBJS = $(patsubst %.cpp,%.o,$(SRCS)) 

Parser.exe: $(OBJS)
	g++ -std=c++11 $^ Shape.o ShapesDB.o -o $@
%.o:	%.cpp
	g++ -std=c++11 -c -g $< -o $@
clean:
	rm -f $(OBJS)  Parser.exe core

