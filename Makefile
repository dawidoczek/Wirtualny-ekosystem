CXX ?= g++
CXXFLAGS ?= -Wall -Wextra -g3
LDLIBS ?= -lncurses
TARGET := output/main
SOURCES := $(wildcard *.cpp)
OBJDIR := build
OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) | output
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDLIBS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

output:
	mkdir -p output

$(OBJDIR):
	mkdir -p $(OBJDIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(OBJECTS) $(TARGET)
	rm -rf $(OBJDIR)
