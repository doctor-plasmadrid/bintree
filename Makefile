CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = tests
SOURCES = tests.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
	rm -rf docs/ html/ latex/

doc:
	doxygen Doxyfile

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean doc run
