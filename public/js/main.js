import { Puzzle } from './Puzzle.js';
import { Board } from './Board.js';
import { Solver } from './Solver.js';

// Define independent width and height
const BOARD_WIDTH = 4; // Change to 3 or 4 to see the IDA* button appear
const BOARD_HEIGHT = 4;

let puzzle, board, solver, wasmModule, gameInstance;

// IDA* Pattern Database Configurations
const IDA_CONFIGS = {
    9: { // 3x3 Puzzle
        name: "8-reg",
        patterns: [
            [1, 2, 3, 4, 5, 6, 7, 8, 0]
        ]
    },
    12: { // 3x4 or 4x3 Puzzle
        name: "443-reg",
        patterns: [
            [1, 2, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0],
            [0, 0, 3, 4, 0, 0, 7, 8, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 11, 0]
        ]
    },
    16: { // 4x4 Puzzle
        name: "555-reg",
        patterns: [
            [1, 2, 3, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 4, 0, 0, 7, 8, 0, 0, 11, 12, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 13, 14, 15, 0]
        ]
    }
};

async function init() {
    wasmModule = await createGameModule();
    
    // Pass both width and height to C++
    gameInstance = new wasmModule.WebGame(BOARD_WIDTH, BOARD_HEIGHT);

    // Pass both width and height to our UI handlers
    puzzle = new Puzzle(gameInstance, BOARD_WIDTH, BOARD_HEIGHT);
    board = new Board('board-container', puzzle);
    solver = new Solver(puzzle, board);

    board.render();
    checkIdaSupport();
    setupEvents();
}

function checkIdaSupport() {
    const area = BOARD_WIDTH * BOARD_HEIGHT;
    const idaBtn = document.getElementById('btn-ida-solve');
    
    // Show the button only if the grid size matches our pre-loaded PDBs
    if (IDA_CONFIGS[area]) {
        idaBtn.style.display = 'inline-block';
    } else {
        idaBtn.style.display = 'none';
    }
}

function setupEvents() {
    document.getElementById('btn-shuffle').addEventListener('click', () => {
        if (solver.isSolving) return;
        puzzle.shuffle(100);
        board.render();
    });

    document.getElementById('btn-solve').addEventListener('click', () => {
        solver.playSolution();
    });

    // --- NEW: IDA* Auto-Solve Event ---
    document.getElementById('btn-ida-solve').addEventListener('click', () => {
        if (solver.isSolving) return;
        if (puzzle.isSolved()) {
            document.getElementById('status').innerText = "Already solved!";
            return;
        }

        const area = BOARD_WIDTH * BOARD_HEIGHT;
        const config = IDA_CONFIGS[area];
        const btn = document.getElementById('btn-ida-solve');
        
        btn.innerText = "Thinking...";
        document.getElementById('status').innerText = "IDA* is searching for the optimal path...";
        
        // Timeout to allow the UI to update the button text before WASM blocks the thread
        setTimeout(() => {
            try {
                // 1. Convert JS Array to std::vector<std::vector<int>> using Embind
                const cppPatterns = new wasmModule.VectorVectorInt();
                for (let row of config.patterns) {
                    const cppRow = new wasmModule.VectorInt();
                    for (let val of row) {
                        cppRow.push_back(val);
                    }
                    cppPatterns.push_back(cppRow);
                    cppRow.delete(); 
                }

                // 2. Initialize the DB (Loads from virtual /databases/ folder)
                gameInstance.initIdaSolver(config.name, cppPatterns);
                cppPatterns.delete();

                // 3. Run the solver
                const cppSolution = gameInstance.idaSolve();
                
                // 4. Extract solution and clean up memory
                const solutionMoves = [];
                for (let i = 0; i < cppSolution.size(); i++) {
                    solutionMoves.push(cppSolution.get(i));
                }
                cppSolution.delete(); 

                if (solutionMoves.length > 0) {
                    document.getElementById('status').innerText = `Optimal Solution found in ${solutionMoves.length} moves!`;
                    playIdaSolution(solutionMoves);
                } else {
                    document.getElementById('status').innerText = "No solution possible (Parity check failed)!";
                }
            } catch (err) {
                console.error("IDA* Solver error:", err);
                document.getElementById('status').innerText = "Error running IDA*. Check console.";
            } finally {
                btn.innerText = "IDA* Solve";
            }
        }, 50);
    });

    window.addEventListener('keydown', (e) => {
        if (solver.isSolving || puzzle.isSolved()) return;
        
        let moved = false;
        if (e.key === 'ArrowUp')    moved = puzzle.move(-2);
        if (e.key === 'ArrowDown')  moved = puzzle.move(2);
        if (e.key === 'ArrowLeft')  moved = puzzle.move(1);
        if (e.key === 'ArrowRight') moved = puzzle.move(-1);

        if (moved) board.render();
    });
}

// Helper function to animate the raw integer array returned by IDA*
function playIdaSolution(movesArray) {
    solver.isSolving = true;
    let i = 0;
    
    // Map IDA* Enum output to core game movement keys
    // Adjust these depending on how your core move logic tracks directional enum integers
    const idaToCoreMap = {
        0: 2,  // IDA Blank Up -> Tile moves Down
        1: 1,  // IDA Blank Right -> Tile moves Left
        2: -2, // IDA Blank Down -> Tile moves Up
        3: -1  // IDA Blank Left -> Tile moves Right
    };

    function nextMove() {
        if (i >= movesArray.length) {
            solver.isSolving = false;
            document.getElementById('status').innerText = "Solved!";
            return;
        }
        
        const rawDir = movesArray[i];
        puzzle.move(idaToCoreMap[rawDir]);
        board.render();
        i++;
        setTimeout(nextMove, 150); // 150ms delay between tiles
    }
    
    nextMove();
}

window.onload = init;