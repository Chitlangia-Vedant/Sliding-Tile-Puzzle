# Compiler
CXX = emcc

# NEW: Updated IDA Source Files with semantic naming
IDA_SOURCES = src/IDAsolver/src/SearchNode.cpp src/IDAsolver/src/SearchNodeRect.cpp \
              src/IDAsolver/src/DisjointDatabase.cpp \
              src/IDAsolver/src/Idastar.cpp src/IDAsolver/src/Pattern.cpp \
              src/IDAsolver/src/Util.cpp src/IDAsolver/src/WalkingDistance.cpp

# Core Source Files + The Bridge
SOURCES = src/core/Board.cpp src/core/Tile.cpp src/core/Direction.cpp \
          src/core/Point.cpp src/solver/Solver.cpp src/wasm/WebGame.cpp \
          src/wasm/IdaBridge.cpp $(IDA_SOURCES)

INCLUDES = -Isrc/core -Isrc/solver -Isrc/IDAsolver/include

# Emscripten Flags for WebAssembly
FLAGS = -O3 -lembind -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createGameModule" \
        --preload-file src/IDAsolver/database@/databases

# Output target
TARGET = public/game.js

all:
	$(CXX) $(SOURCES) $(INCLUDES) -o $(TARGET) $(FLAGS)
	@echo "WebAssembly build complete! Files generated in public/ folder."

clean:
	rm -f public/game.js public/game.wasm public/game.data