tagos
===

Successfully built on clean install of Ubuntu 16.04 (10 May 2018)

## Dependencies
- **sfml** 2.3
- **boost** 1.58
- **make**
- **c++11 compiler** (g++ or clang)

## Building the engine

NOTE: building takes a lot of memory!

First get sfml and boost: `sudo apt install libsfml-dev libboost-all-dev`

- If your computer doesn't have a large amount of RAM, follow these steps to use clang instead of g++:
    
    `sudo apt install clang`
    
    `export CXX=clang++` 
        
Now, build the engine using the makefile.

To build normally: `make -j`

To build debug mode: `make debug -j`

To build release mode: `make release -j`

To build WITHOUT rendering enabled: `make [debug|release] -j CXXFLAGS=-DDISABLE_RENDER`

## Setup testing

You'll need something to test the engine on. Convert some old TagPro data to new OSCTF data by running these two commands:
    
`./scripts/export_all_maps.sh`

`./scripts/export_all_textures.sh`

Next, install nvm. As of May 10 2018, the latest version is 0.33.11.

You'll need curl, so if you don't have it already, `apt install curl`.

Then run `curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash`

(You can visit http://nvm.sh for more detailed instructions and documentation on nvm.)

After installing nvm, close and reopen your terminal to gain access to the node commands.

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

To render map format: `./tagos render <MAP_FILE>`

## Running tests

Open a new terminal window.

`cd tests`

`node <testfile.js>`

## Cleaning

To clean the build files: `make mostlyclean`

To clean everything including built libs: `make clean`

## Miscellaneous

To convert map format: `./tagos export <IN_JSON> <IN_PNG> <OUT_JSON>`
