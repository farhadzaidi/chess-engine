CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
SRCDIR = src
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))
EXEC = $(BUILDDIR)/main
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

DEPTH ?= 5

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(SFML_LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: run perft clean

run: $(EXEC)
	./$(EXEC)

perft: $(EXEC)
	./$(EXEC) perft $(DEPTH)

clean:
	rm -rf $(BUILDDIR)