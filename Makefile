CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
INCLUDES = -Iheaders

TARGET = main
SRCS = main.cpp
OBJS = $(addprefix build/,$(SRCS:.cpp=.o))

all: build $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f build/*.o $(TARGET)
