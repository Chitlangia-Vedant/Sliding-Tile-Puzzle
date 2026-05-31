export class IdaSolver {
    constructor(puzzle, board) {
        this.puzzle = puzzle;
        this.board = board;
        this.isSolving = false;
        this.worker = null;

        // Moved from main.js
        this.IDA_CONFIGS = {
            9: { 
                name: "8-reg",
                patterns: [[1, 2, 3, 4, 5, 6, 7, 8, 0]]
            },
            12: { 
                name: "443-reg",
                patterns: [
                    [1, 2, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0],
                    [0, 0, 3, 4, 0, 0, 7, 8, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 11, 0]
                ]
            },
            16: { 
                name: "555-reg",
                patterns: [
                    [1, 2, 3, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 4, 0, 0, 7, 8, 0, 0, 11, 12, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 13, 14, 15, 0]
                ]
            }
        };

        this.initWorker();
        this.checkSupport();
    }

    initWorker() {
        this.worker = new Worker('js/worker.js?v=' + Date.now());
        
        this.worker.onmessage = (e) => {
            if (e.data.type === 'READY') {
                const area = this.puzzle.width * this.puzzle.height;
                const config = this.IDA_CONFIGS[area];
                if (config) {
                    this.worker.postMessage({ 
                        type: 'INIT_IDA', 
                        config: { name: config.name, width: this.puzzle.width, height: this.puzzle.height, patterns: config.patterns } 
                    });
                }
            } else if (e.data.type === 'SOLUTION') {
                const solutionMoves = e.data.solutionMoves;
                const btn = document.getElementById('btn-ida-solve');
                btn.innerText = "IDA* Solve";

                if (solutionMoves.length > 0) {
                    document.getElementById('status').innerText = `Optimal Solution found in ${solutionMoves.length} moves!`;
                    this.playSolution(solutionMoves); 
                } else {
                    document.getElementById('status').innerText = "No solution possible (Parity check failed)!";
                    this.isSolving = false;
                }
            } else if (e.data.type === 'ERROR') {
                console.error("Worker error:", e.data.error);
                document.getElementById('status').innerText = "Error running IDA*. Check console.";
                document.getElementById('btn-ida-solve').innerText = "IDA* Solve";
                this.isSolving = false;
            }
        };
    }

    checkSupport() {
        const area = this.puzzle.width * this.puzzle.height;
        const idaBtn = document.getElementById('btn-ida-solve');
        if (this.IDA_CONFIGS[area]) {
            idaBtn.style.display = 'inline-block';
        } else {
            idaBtn.style.display = 'none';
        }
    }

    solve() {
        if (this.isSolving) return;
        if (this.puzzle.isSolved()) {
            document.getElementById('status').innerText = "Already solved!";
            return;
        }

        this.isSolving = true;
        document.getElementById('btn-ida-solve').innerText = "Thinking...";
        document.getElementById('status').innerText = "IDA* is searching in the background...";
        
        this.worker.postMessage({
            type: 'SOLVE',
            gridArray: this.puzzle.getGrid(),
            width: this.puzzle.width,
            height: this.puzzle.height
        });
    }

    playSolution(movesArray) {
        let i = 0;

        // Use arrow function to preserve 'this' context
        const nextMove = () => {
            if (i >= movesArray.length) {
                this.isSolving = false;
                document.getElementById('status').innerText = "Solved!";
                return;
            }
            
            const coreDir = movesArray[i];
            this.puzzle.applyMove(coreDir); 
            
            this.board.render();
            i++;
            setTimeout(nextMove, 150); 
        };
        
        nextMove();
    }
}