CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRCS = $(wildcard ./*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = main

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: ./%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

.PHONY: run clean

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
