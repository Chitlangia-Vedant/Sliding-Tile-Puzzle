export type SolverId = 'fringe' | 'ida';
export type MoveDirection = 'U' | 'D' | 'L' | 'R';

export interface WebGame {
  shuffle: (moves: number) => void;
  move: (direction: number) => boolean;
  applyMove: (direction: number) => boolean;
  moveTileAt?: (x: number, y: number) => number;
  moveTileNumber?: (tile: number) => number;
  isTileCorrectAt?: (x: number, y: number) => boolean;
  isTileCorrectNumber?: (tile: number) => boolean;
  getCorrectTiles?: () => unknown;
  setGrid?: (grid: unknown) => boolean;
  getTile: (x: number, y: number) => number;
  isSolved: () => boolean;
  autoSolve: () => unknown;
  getGrid: () => unknown;
  delete?: () => void;
  isSolvable: () => boolean;
}

export interface MoveEntry {
  id: number;
  direction: MoveDirection;
}

export interface IdaConfig {
  name: string;
  patterns: number[][];
}
