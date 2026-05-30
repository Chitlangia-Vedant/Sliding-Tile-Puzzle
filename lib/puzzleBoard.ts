import type { WasmModule } from '@/hooks/useWasm';

import type { WebGame } from './puzzleTypes';
import { maxDimension, minDimension } from './puzzleConfig';

export const formatTime = (seconds: number) => {
  const minutes = Math.floor(seconds / 60);
  const remainingSeconds = seconds % 60;
  return `${String(minutes).padStart(2, '0')}:${String(remainingSeconds).padStart(2, '0')}`;
};

export const clampDimension = (value: number) =>
  Math.min(maxDimension, Math.max(minDimension, Number.isFinite(value) ? value : minDimension));

export const sleep = (ms: number) => new Promise((resolve) => window.setTimeout(resolve, ms));

export const readBoard = (game: WebGame, rows: number, cols: number) =>
  Array.from({ length: rows * cols }, (_, index) => {
    const tile = game.getTile(index % cols, Math.floor(index / cols));
    return tile === 0 ? null : tile;
  });

export const createGame = (module: WasmModule, rows: number, cols: number) =>
  new module.WebGame(cols, rows) as WebGame;
