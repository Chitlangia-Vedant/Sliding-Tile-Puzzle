import { Puzzle } from './Puzzle.js';
import { Board } from './Board.js';
import { Solver } from './Solver.js';
import { IdaSolver } from './IdaSolver.js'; // NEW: Import your new class

const BOARD_WIDTH = 4; 
const BOARD_HEIGHT = 4;

let puzzle, board, solver, idaSolver, wasmModule, gameInstance;

async function init() {
    wasmModule = await createGameModule();
    gameInstance = new wasmModule.WebGame(BOARD_WIDTH, BOARD_HEIGHT);
    puzzle = new Puzzle(gameInstance, BOARD_WIDTH, BOARD_HEIGHT);
    board = new Board('board-container', puzzle);
    solver = new Solver(puzzle, board);
    
    // NEW: Instantiate the isolated IDA solver
    idaSolver = new IdaSolver(puzzle, board); 

    board.render();
    setupEvents();
}

function setupEvents() {
    document.getElementById('btn-shuffle').addEventListener('click', () => {
        // Prevent shuffling if EITHER solver is running
        if (solver.isSolving || idaSolver.isSolving) return;
        puzzle.shuffle(100);
        board.render();
    });

    document.getElementById('btn-solve').addEventListener('click', () => {
        if (idaSolver.isSolving) return; // Prevent double-clicking
        solver.playSolution();
    });

    document.getElementById('btn-ida-solve').addEventListener('click', () => {
        if (solver.isSolving) return; // Prevent double-clicking
        idaSolver.solve(); // Delegate logic to the class
    });

    window.addEventListener('keydown', (e) => {
        // Prevent manual moves if EITHER solver is running
        if (solver.isSolving || idaSolver.isSolving || puzzle.isSolved()) return;
        
        let moved = false;
        if (e.key === 'ArrowUp')    moved = puzzle.move(-2);
        if (e.key === 'ArrowDown')  moved = puzzle.move(2);
        if (e.key === 'ArrowLeft')  moved = puzzle.move(1);
        if (e.key === 'ArrowRight') moved = puzzle.move(-1);

        if (moved) board.render();
    });
}

window.onload = init;