CXXFLAGS=-std=c++11 -Wall -Wextra -D_DEBUG
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lBox2D -lboost_system -lboost_thread -pthread

all: build/tagos

build/tagos: build/tagos.o build/tp_map_importer.o build/gate.o build/gate_type.o build/map.o build/map_type.o build/toggle.o build/toggle_tag_type.o build/toggle_tag.o build/color.o build/util.o build/polygon.o build/settings.o build/ball.o build/wall.o build/flag.o build/flag_type.o build/booster.o build/booster_type.o build/powerup.o build/spike.o build/bomb.o build/portal.o build/spawn.o build/spawn_type.o build/tile.o build/tile_type.o build/websocket_server.o build/lodepng.o
	$(CXX) $^ -o $@ $(LDFLAGS)

build/tagos.o: src/tagos.cpp src/libs/json.hpp src/libs/INIReader.h 
	$(CXX) $(CXXFLAGS) src/tagos.cpp -c -o build/tagos.o

build/tp_map_importer.o: src/tp_map_importer.cpp src/tp_map_importer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/gate.o: src/gate.cpp src/gate.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/gate_type.o: src/gate_type.cpp src/gate_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/map.o: src/map.cpp src/map.hpp src/contact_listener.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/map_type.o: src/map_type.cpp src/map_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/toggle.o: src/toggle.cpp src/toggle.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/toggle_tag_type.o: src/toggle_tag_type.cpp src/toggle_tag_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/toggle_tag.o: src/toggle_tag.cpp src/toggle_tag.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/color.o: src/color.cpp src/color.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/util.o: src/util.cpp src/util.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/polygon.o: src/polygon.cpp src/polygon.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/settings.o: src/settings.cpp src/settings.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/ball.o: src/ball.cpp src/ball.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/wall.o: src/wall.cpp src/wall.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/flag.o: src/flag.cpp src/flag.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/flag_type.o: src/flag_type.cpp src/flag_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/booster.o: src/booster.cpp src/booster.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/booster_type.o: src/booster_type.cpp src/booster_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/powerup.o: src/powerup.cpp src/powerup.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/spike.o: src/spike.cpp src/spike.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/bomb.o: src/bomb.cpp src/bomb.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/portal.o: src/portal.cpp src/portal.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/spawn.o: src/spawn.cpp src/spawn.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/spawn_type.o: src/spawn_type.cpp src/spawn_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/tile.o: src/tile.cpp src/tile.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/tile_type.o: src/tile_type.cpp src/tile_type.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/websocket_server.o: src/websocket_server.cpp src/websocket_server.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/lodepng.o: src/libs/lodepng.cpp src/libs/lodepng.h
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY: clean

clean:
	@rm build/*
