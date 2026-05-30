import type { MoveDirection, SolverId } from './puzzleTypes';

export const initialRows = 4;
export const initialCols = 4;
export const minDimension = 2;
export const maxDimension = 10;
export const shuffleMoves = 100;
export const solveStepDelay = 100;

export const directionLabels: Record<number, MoveDirection> = {
  '-2': 'U',
  '-1': 'R',
  '1': 'L',
  '2': 'D',
};

export const solverOptions: { id: SolverId; label: string }[] = [
  { id: 'fringe', label: 'Fringe' },
  { id: 'ida', label: 'IDA* + Pattern database' },
];
