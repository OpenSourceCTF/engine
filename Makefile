override CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -Werror=switch
override LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lboost_system -lboost_thread -pthread

BUILD := build
SRC := src
INCLUDE := include

SOURCES := $(wildcard $(SRC)/*.cpp)
SOURCES := $(filter-out $(SRC)/tagos.cpp, $(SOURCES))
OBJECTS := $(SOURCES:$(SRC)/%.cpp=$(BUILD)/%.o)

all: tagos

release: CXXFLAGS += -O3
release: tagos
debug: CXXFLAGS += -DDEBUG -g
debug: tagos

tagos: $(OBJECTS) $(BUILD)/tagos.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD)/tagos.o: $(SRC)/tagos.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $(SRC)/tagos.cpp -c -o $(BUILD)/tagos.o

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $< -c -o $@

.PHONY: clean

clean:
	@rm -f build/*
