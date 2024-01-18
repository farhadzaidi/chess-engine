CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
SRCDIR = src
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))
EXEC = $(BUILDDIR)/main
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SFML_LIBS_BREW =

DEPTH ?= 5

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CXXFLAGS += -I/opt/homebrew/Cellar/sfml/2.6.1/include
	SFML_LIBS_BREW += -L/opt/homebrew/Cellar/sfml/2.6.1/lib
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(SFML_LIBS_BREW) $(SFML_LIBS)

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