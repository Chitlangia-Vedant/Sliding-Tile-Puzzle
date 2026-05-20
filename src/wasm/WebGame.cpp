#include <emscripten/bind.h>
#include "Board.h"
#include "Solver.h"
#include "Logger.h" 
#include "IdaBridge.h"
#include <vector>
#include <string>

using namespace emscripten;

class WebGame {
private:
    Board m_board;

public:
    // Update constructor to take width and height
    WebGame(int width, int height) : m_board(width, height) {
        Logger::info("WebGame initialized with board size: " + std::to_string(width) + "x" + std::to_string(height));
    }

    void shuffle(int moves) {
        Logger::info("UI EVENT: Shuffle button pressed.");
        m_board.random(moves);
        Logger::info("UI EVENT: Shuffle sequence completely finished.");
    }

    bool move(int dirInt) {
        Direction dir{dirInt};
        return m_board.moveTile(dir);
    }

    int getTile(int x, int y) {
        return m_board.getTileNum(x, y);
    }

    // Replace getSize with getWidth and getHeight
    int getWidth() const { return m_board.getWidth(); }
    int getHeight() const { return m_board.getHeight(); }

    bool isSolved() {
        return m_board.solved();
    }

    std::vector<int> autoSolve() {
        Logger::info("UI EVENT: Auto-Solve button pressed.");
        
        Solver solver{m_board};
        std::vector<Direction> solutionDirs = solver.solve();
        
        Logger::info("UI EVENT: Auto-Solve complete. Handing solution vector back to JavaScript.");
        
        std::vector<int> jsSolution;
        for (Direction d : solutionDirs) {
            jsSolution.push_back(d.getDir());
        }
        return jsSolution;
    }
    void initIdaSolver(std::string dbName, std::vector<std::vector<int>> patterns) {
        Logger::info("Initializing IDA* databases...");
        initIda(dbName, m_board.getWidth(), m_board.getHeight(), patterns);
    }

    // NEW: Function to execute the IDA solver
    std::vector<int> idaSolve() {
        Logger::info("UI EVENT: IDA* Auto-Solve button pressed.");
        std::vector<int> grid = m_board.toSolverGrid();
        
        std::vector<int> solutionDirs = runIda(grid, m_board.getWidth(), m_board.getHeight());
        
        if(solutionDirs.empty() && !m_board.solved()) {
             Logger::warn("IDA* Solver reported puzzle as unsolvable or already solved.");
        }
        return solutionDirs;
    }
};

// --- Emscripten Binding Block ---
EMSCRIPTEN_BINDINGS(my_game_module) {
    register_vector<int>("VectorInt");
    register_vector<std::vector<int>>("VectorVectorInt");

    class_<WebGame>("WebGame")
        .constructor<int, int>() // Now takes two integers!
        .function("shuffle", &WebGame::shuffle)
        .function("move", &WebGame::move)
        .function("getTile", &WebGame::getTile)
        .function("getWidth", &WebGame::getWidth)
        .function("getHeight", &WebGame::getHeight)
        .function("isSolved", &WebGame::isSolved)
        .function("autoSolve", &WebGame::autoSolve)
        .function("initIdaSolver", &WebGame::initIdaSolver)
        .function("idaSolve", &WebGame::idaSolve);
}