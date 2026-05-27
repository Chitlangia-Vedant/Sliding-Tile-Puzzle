export class Tile {
    static create(num) {
        const el = document.createElement('div');
        el.className = num === 0 ? 'tile empty' : 'tile';
        el.innerText = num === 0 ? '' : num;
        return el;
    }
}