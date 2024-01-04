CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = main

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(SFML_LIBS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: run clean

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
