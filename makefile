CXX    = g++
FILE   = $(wildcard *.cpp)
TARGET = $(patsubst %.cpp,%,$(FILE))
FLAGS  = -O3 -std=c++17

all:
	$(CXX) $(FLAGS) $(FILE) -o $(TARGET)
clean:
	rm -f $(TARGET)
