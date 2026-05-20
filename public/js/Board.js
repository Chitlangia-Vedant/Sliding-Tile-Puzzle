import { Tile } from './Tile.js';

export class Board {
    constructor(containerId, puzzle) {
        this.container = document.getElementById(containerId);
        this.puzzle = puzzle;
        this.statusEl = document.getElementById('status');
        
        // Dynamically set CSS grid columns based on WIDTH
        this.container.style.gridTemplateColumns = `repeat(${puzzle.width}, 80px)`;
    }

    render() {
        this.container.innerHTML = ''; 
        
        // Loop uses height for Y, and width for X
        for (let y = 0; y < this.puzzle.height; y++) {
            for (let x = 0; x < this.puzzle.width; x++) {
                const tileNum = this.puzzle.getTile(x, y);
                const tileEl = Tile.create(tileNum);
                this.container.appendChild(tileEl);
            }
        }
        
        this.updateStatus();
    }

    updateStatus() {
        if (this.puzzle.isSolved()) {
            this.statusEl.innerText = "Puzzle Solved!";
        } else {
            this.statusEl.innerText = "";
        }
    }
}