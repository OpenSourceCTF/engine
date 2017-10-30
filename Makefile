override CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic
override LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lboost_system -lboost_thread -pthread

BUILD := build
SRC := src
LIB := src/libs

SOURCES := $(wildcard $(SRC)/*.cpp)
SOURCES := $(filter-out $(SRC)/tagos.cpp, $(SOURCES))
OBJECTS := $(SOURCES:$(SRC)/%.cpp=$(BUILD)/%.o)
LIBSRC  := $(wildcard $(LIB)/*.cpp)
LIBOBJ  := $(LIBSRC:$(LIB)/%.cpp=$(LIB)/obj/%.o)

all: tagos

release: CXXFLAGS += -O3
release: tagos
debug: CXXFLAGS += -DDEBUG -g
debug: tagos

tagos: $(OBJECTS) $(BUILD)/tagos.o $(LIBOBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD)/tagos.o: $(SRC)/tagos.cpp
	$(CXX) $(CXXFLAGS) -I./$(LIB) $(SRC)/tagos.cpp -c -o $(BUILD)/tagos.o

$(BUILD)/%.o: $(SRC)/%.cpp $(LIBOBJ)
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(LIB)/obj/%.o: $(LIB)/%.cpp
	$(CXX) $(CXXFLAGS) -I./$(LIB) $< -c -o $@

.PHONY: clean

clean:
	@rm build/*
