CXX       = g++
CPP_FILES = $(wildcard *.cpp)
OBJS      = $(patsubst %.cpp,%.o,$(CPP_FILES))
CXXFLAGS  = -std=c++17 -O3 -Wall -Wextra -Werror
TARGET    = huffman

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET)
