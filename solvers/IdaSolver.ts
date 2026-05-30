import type { WasmModule } from '@/hooks/useWasm';
import type { IdaConfig, WebGame } from '@/lib/puzzleTypes';
import { createVectorVectorInt, vectorToArray } from '@/lib/wasmVectors';

const idaConfigs: Record<number, IdaConfig> = {
  9: {
    name: '8-reg',
    patterns: [[1, 2, 3, 4, 5, 6, 7, 8, 0]],
  },
  12: {
    name: '443-reg',
    patterns: [
      [1, 2, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0],
      [0, 0, 3, 4, 0, 0, 7, 8, 0, 0, 0, 0],
      [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 11, 0],
    ],
  },
  16: {
    name: '555-reg',
    patterns: [
      [1, 2, 3, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
      [0, 0, 0, 4, 0, 0, 7, 8, 0, 0, 11, 12, 0, 0, 0, 0],
      [0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 13, 14, 15, 0],
    ],
  },
};

export class IdaSolver {
  private initialized = new Set<string>();

  getConfig(rows: number, cols: number) {
    return idaConfigs[rows * cols] ?? null;
  }

  isAvailable(rows: number, cols: number) {
    return Boolean(this.getConfig(rows, cols));
  }

  solve(game: WebGame, module: WasmModule, rows: number, cols: number) {
    const config = this.getConfig(rows, cols);

    if (
      !config ||
      typeof module.initIda !== 'function' ||
      typeof module.runIda !== 'function'
    ) {
      return null;
    }

    const initKey = `${config.name}:${cols}x${rows}`;
    if (!this.initialized.has(initKey)) {
      const patterns = createVectorVectorInt(module, config.patterns);
      try {
        module.initIda(config.name, cols, rows, patterns.vector);
      } finally {
        patterns.delete();
      }
      this.initialized.add(initKey);
    }

    const grid = game.getGrid();
    const solution = module.runIda(grid, cols, rows);
    const solutionArray = vectorToArray(solution);
    (solution as { delete?: () => void })?.delete?.();
    (grid as { delete?: () => void })?.delete?.();

    return solutionArray;
  }
}
