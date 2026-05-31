export class Puzzle {
    constructor(wasmInstance, width, height) {
        this.api = wasmInstance;
        this.width = width;
        this.height = height;
    }

    shuffle(moves) { this.api.shuffle(moves); }
    move(dir) { return this.api.move(dir); }
    applyMove(dir) { return this.api.applyMove(dir); }
    getTile(x, y) { return this.api.getTile(x, y); }
    isSolved() { return this.api.isSolved(); }

    autoSolve() {
        let solutionVector = this.api.autoSolve();
        let moves = [];
        for (let i = 0; i < solutionVector.size(); i++) moves.push(solutionVector.get(i));
        solutionVector.delete(); 
        return moves;
    }

    // NEW: Extract the grid array to pass to the worker
    getGrid() {
        let gridVector = this.api.getGrid();
        let grid = [];
        for (let i = 0; i < gridVector.size(); i++) {
            grid.push(gridVector.get(i));
        }
        gridVector.delete();
        return grid;
    }
}