override CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -Werror=switch -Wno-inconsistent-missing-override
override LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system \
	-Llib/Box2D/Build/gmake/bin/Debug -lBox2D \
	-lboost_system -lboost_thread \
	-pthread

BUILD := build
SRC := src
INCLUDE := -Iinclude -Ilib/Box2D

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(SOURCES:$(SRC)/%.cpp=$(BUILD)/%.o)

all: tagos

release: CXXFLAGS += -O3
release: tagos
debug: CXXFLAGS += -DDEBUG -g
debug: tagos

tagos: $(OBJECTS) lib/Box2D/Build/gmake/bin/Debug/libBox2D.a
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@

lib/Box2D/Build/gmake/bin/Debug/libBox2D.a:
	make -C lib/Box2D/Build/gmake

.PHONY: clean

clean:
	@rm -f build/*
