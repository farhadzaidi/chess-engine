CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRCDIR = src
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))
EXEC = $(BUILDDIR)/main
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(SFML_LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: run clean

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf $(BUILDDIR)
