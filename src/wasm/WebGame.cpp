#include <emscripten/bind.h>
#include "Board.h"
#include "Solver.h"
#include "Logger.h" 
#include "IdaBridge.h"
#include <vector>
#include <string>
#include <sstream>

using namespace emscripten;

class WebGame {
private:
    Board m_board;

public:
    WebGame(int width, int height) : m_board(width, height) {
        std::ostringstream oss;
        oss << "WebGame initialized with board size: " << width << "x" << height;
        Logger::info(oss.str());
    }

    void shuffle(int moves) { m_board.random(moves); }

    bool move(int dirInt) { 
        Direction d{dirInt};
        Point empty = m_board.getEmptyPoint();
        Point target = empty.getAdjacentPoint(-d);
        
        if (target.getXcoord() < 0 || target.getXcoord() >= m_board.getWidth() ||
            target.getYcoord() < 0 || target.getYcoord() >= m_board.getHeight()) {
            return false;
        }

        int tileNum = m_board.getTileNum(target.getXcoord(), target.getYcoord());
        int targetX = empty.getXcoord();
        int targetY = empty.getYcoord();
        
        bool success = m_board.moveTile(d);
        if (success) {
            std::ostringstream oss;
            oss << "[Manual] [" << tileNum << "]->(" << targetX << "," << targetY << "): " << d.toChar();
            Logger::debug(oss.str());
        }
        return success;
    }

    int moveTileAt(int x, int y) {
        int tileNum = m_board.getTileNum(x, y);
        Point empty = m_board.getEmptyPoint();
        
        int dirInt = m_board.moveTileAt(x, y);
        
        // If dirInt is not 0, the move was successful
        if (dirInt != 0) {
            Direction d{dirInt}; // Cast back to Direction to use toChar()
            std::ostringstream oss;
            oss << "[Manual] [" << tileNum << "]->(" << empty.getXcoord() << "," << empty.getYcoord() << "): " << d.toChar();
            Logger::debug(oss.str());
        }
        return dirInt;
    }

    int moveTileNumber(int tile) {
        Point empty = m_board.getEmptyPoint();
        
        int dirInt = m_board.moveTileNumber(tile);
        
        if (dirInt != 0) {
            Direction d{dirInt}; // Cast back to Direction to use toChar()
            std::ostringstream oss;
            oss << "[Manual] [" << tile << "]->(" << empty.getXcoord() << "," << empty.getYcoord() << "): " << d.toChar();
            Logger::debug(oss.str());
        }
        return dirInt;
    }

    bool applyMove(int dirInt) {
        return m_board.moveTile(Direction{dirInt});
    }
    int getTile(int x, int y) { return m_board.getTileNum(x, y); }
    int getWidth() const { return m_board.getWidth(); }
    int getHeight() const { return m_board.getHeight(); }
    bool isSolvable() const { return m_board.isSolvable(); }
    bool isSolved() { return m_board.solved(); }
    bool isTileCorrectAt(int x, int y) const { return m_board.isTileCorrect(x, y); }
    bool isTileCorrectNumber(int t) const { return m_board.isTileCorrect(t); }
    std::vector<int> getCorrectTiles() const { return m_board.getCorrectTiles(); }

    std::vector<int> autoSolve() {
        Solver solver{m_board};
        std::vector<Direction> solutionDirs = solver.solve();
        std::vector<int> jsSolution;
        for (Direction d : solutionDirs) jsSolution.push_back(d.getDir());
        return jsSolution;
    }

    // NEW: Get the current grid state to send to the Web Worker
    std::vector<int> getGrid() const {
        return m_board.toSolverGrid();
    }
    bool setGrid(const std::vector<int>& grid) {
    return m_board.setGrid(grid);
}
};

// --- Emscripten Binding Block ---
EMSCRIPTEN_BINDINGS(my_game_module) {
    register_vector<int>("VectorInt");
    register_vector<std::vector<int>>("VectorVectorInt");

    class_<WebGame>("WebGame")
        .constructor<int, int>()
        .function("shuffle", &WebGame::shuffle)
        .function("move", &WebGame::move)
        .function("applyMove", &WebGame::applyMove)
        .function("getTile", &WebGame::getTile)
        .function("getWidth", &WebGame::getWidth)
        .function("getHeight", &WebGame::getHeight)
        .function("isSolved", &WebGame::isSolved)
        .function("autoSolve", &WebGame::autoSolve)
        .function("getGrid", &WebGame::getGrid)
        .function("setGrid", &WebGame::setGrid)
        .function("isSolvable", &WebGame::isSolvable) // Bound for main thread
        .function("moveTileAt", &WebGame::moveTileAt)
        .function("moveTileNumber", &WebGame::moveTileNumber)
        .function("isTileCorrectAt", &WebGame::isTileCorrectAt)
        .function("isTileCorrectNumber", &WebGame::isTileCorrectNumber)
        .function("getCorrectTiles", &WebGame::getCorrectTiles);
    // NEW: Expose standalone IDA bridge functions for the Web Worker
    function("initIda", &initIda);
    function("runIda", &runIda);
}