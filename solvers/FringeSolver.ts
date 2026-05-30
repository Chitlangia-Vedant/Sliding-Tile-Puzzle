import type { WebGame } from '@/lib/puzzleTypes';
import { vectorToArray } from '@/lib/wasmVectors';

export class FringeSolver {
  solve(game: WebGame) {
    const solution = game.autoSolve();
    const solutionArray = vectorToArray(solution);
    (solution as { delete?: () => void })?.delete?.();

    return solutionArray;
  }
}
