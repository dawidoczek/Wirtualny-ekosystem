CXX ?= g++
CXXFLAGS ?= -Wall -Wextra -g3
CPPFLAGS ?= -Iinclude
LDLIBS ?= -lncurses
TARGET := output/main
SOURCES := $(wildcard src/*.cpp)
OBJDIR := build
OBJECTS := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) | output
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJECTS) -o $(TARGET) $(LDLIBS)

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

output:
	mkdir -p output

$(OBJDIR):
	mkdir -p $(OBJDIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(OBJECTS) $(TARGET)
	rm -rf $(OBJDIR)
