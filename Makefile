## 
##   VICTOR MENJÓN FANDOS: 930870
##   DAVID CARRASCO ALONSO: 927253
##

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

TARGET = mainGSenku
SOURCES = mainGSenku.cpp GSenku.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
