export class Puzzle {
    // Add height parameter
    constructor(wasmInstance, width, height) {
        this.api = wasmInstance;
        this.width = width;
        this.height = height;
    }

    shuffle(moves) { this.api.shuffle(moves); }
    
    move(dir) { return this.api.move(dir); }
    
    getTile(x, y) { return this.api.getTile(x, y); }
    
    isSolved() { return this.api.isSolved(); }

    autoSolve() {
        let solutionVector = this.api.autoSolve();
        let moves = [];
        for (let i = 0; i < solutionVector.size(); i++) {
            moves.push(solutionVector.get(i));
        }
        solutionVector.delete(); 
        return moves;
    }
}