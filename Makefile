CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include -I./$(SRCDIR) -I./$(TESTDIR)

SRCDIR = src
TESTDIR = tests
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(TESTDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = main

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: run clean

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
