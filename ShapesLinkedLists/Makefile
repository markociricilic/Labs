SRCS = Parser.cpp Shape.cpp ShapeNode.cpp ShapeList.cpp GroupNode.cpp GroupList.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

Parser.exe: $(OBJS)
	g++ -std=c++11 $^ -o $@
%.o:	%.cpp
	g++ -std=c++11 -c -g $< -o $@
clean:
	rm -f *.o  Parser.exe core

