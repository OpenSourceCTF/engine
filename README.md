tagos
===

Successfully built on clean install of Ubuntu 16.04

## Dependencies
- **sfml** 2.3
- **boost** 1.58
- **make**
- **c++11 compiler** (g++ or clang)

## Building the engine

NOTE: building takes a lot of memory!

`sudo apt-get install libsfml-dev libboost-all-dev`

- If your computer doesn't have a large amount of RAM, follow these steps!
    
    `export CXX=clang++` 
    
    `sudo apt-get install clang`
        
`./scripts/export_all_maps.sh`

`./scripts/export_all_textures.sh`

To build normally: `make -j`

To build debug mode: `make debug -j`

To build release mode: `make release -j`

To build WITHOUT rendering enabled: `make [debug|release] -j CXXFLAGS=-DDISABLE_RENDER`

## Setup testing

Install NVM. As of November 30 2017, the latest version is 0.33.6.

`wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.33.6/install.sh | bash`

At this point, close and reopen your terminal to gain access to the node commands.

`cd tests`

`nvm install node`

`npm install`

## Starting the server

Read through config.ini and edit as needed before starting the server.
You probably won't need to edit anything if you just want to run the server normally (8 games of 8 players with normal physics).

`./tagos serve`

## Rendering

In the tagos console after gamerooms are created:

`render [gameroom#]`

In the renderer, you can use arrow keys to move the viewport, or 'c' to lock the viewport.

To render map format: `./tagos render <MAP_FILE>

## Running tests

Open a new terminal window.

`cd tests`

`node <testfile.js>`

## Cleaning

To clean the build files: `make mostlyclean`

To clean everything including built libs: `make clean`

## Miscellaneous

To convert map format: `./tagos export <IN_JSON> <IN_PNG> <OUT_JSON>`
