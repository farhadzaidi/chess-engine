CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../include -I../src

TESTS = $(wildcard ./*.cpp)
OBJS = $(TESTS:.cpp=.o)
EXEC = test_runner

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: ./%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

.PHONY: run clean

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
