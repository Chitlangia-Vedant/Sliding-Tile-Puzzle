export class Solver {
    constructor(puzzle, board) {
        this.puzzle = puzzle;
        this.board = board;
        this.isSolving = false;
    }

    async playSolution() {
        if (this.isSolving || this.puzzle.isSolved()) return;
        
        this.isSolving = true;
        const moves = this.puzzle.autoSolve();
        
        for (let moveDir of moves) {
            this.puzzle.applyMove(moveDir);
            this.board.render();
            await new Promise(r => setTimeout(r, 80)); // 80ms animation delay
        }
        
        this.isSolving = false;
    }
}