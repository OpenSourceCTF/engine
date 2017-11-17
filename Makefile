override CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -Werror=switch \
	-Wno-inconsistent-missing-override

override LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system \
	-Llib/Box2D/Build/gmake/bin/Debug -lBox2D \
	-lboost_system -lboost_thread \
	-pthread

BUILD_DIR := build
SRC_DIR := src

INCLUDES := -Iinclude -Ilib/Box2D
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: tagos

release: CXXFLAGS += -O3
release: tagos
debug: CXXFLAGS += -DDEBUG -g
debug: tagos

tagos: $(OBJECTS) lib/Box2D/Build/gmake/bin/Debug/libBox2D.a
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -c -o $@

lib/Box2D/Build/gmake/bin/Debug/libBox2D.a:
	make -C lib/Box2D/Build/gmake

.PHONY: mostlyclean clean

clean:
	@rm -f build/*
	make -C lib/Box2D/Build/gmake clean

mostlyclean:
	@rm -f build/*
