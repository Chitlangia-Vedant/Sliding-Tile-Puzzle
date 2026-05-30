'use client';

import { useCallback, useEffect, useRef, useState } from 'react';

import { GameControls } from '@/components/GameControls';
import { GameHeader } from '@/components/GameHeader';
import { GameSidebar } from '@/components/GameSidebar';
import { PuzzleContainer } from '@/components/PuzzleContainer';
import { useWasm } from '@/hooks/useWasm';
import { clampDimension, createGame, readBoard, sleep } from '@/lib/puzzleBoard';
import {
  directionLabels,
  initialCols,
  initialRows,
  shuffleMoves,
  solveStepDelay,
} from '@/lib/puzzleConfig';
import type { MoveEntry, SolverId, WebGame } from '@/lib/puzzleTypes';
import { createVectorInt, vectorToArray } from '@/lib/wasmVectors';
import { FringeSolver } from '@/solvers/FringeSolver';
import { IdaSolver } from '@/solvers/IdaSolver';

export default function Page() {
  const { module: wasmModule, loading: wasmLoading, error: wasmError } = useWasm('/game.wasm');
  const gameRef = useRef<WebGame | null>(null);
  const moveDirectionsRef = useRef<number[]>([]);
  const moveIdRef = useRef(0);
  const fringeSolverRef = useRef(new FringeSolver());
  const idaSolverRef = useRef(new IdaSolver());
  const [sidebarOpen, setSidebarOpen] = useState(true);
  const [rows, setRows] = useState(initialRows);
  const [cols, setCols] = useState(initialCols);
  const [tiles, setTiles] = useState<(number | null)[]>([]);
  const [moves, setMoves] = useState<MoveEntry[]>([]);
  const [correctTiles, setCorrectTiles] = useState<Set<number>>(new Set());
  const [solver, setSolver] = useState<SolverId>('fringe');
  const [elapsedSeconds, setElapsedSeconds] = useState(0);
  const [running, setRunning] = useState(false);
  const [solving, setSolving] = useState(false);
  const [solvable, setSolvable] = useState(true);

  const idaAvailable = idaSolverRef.current.isAvailable(rows, cols);

  const clearRunState = useCallback(() => {
    setMoves([]);
    setElapsedSeconds(0);
    setRunning(false);
    moveDirectionsRef.current = [];
    moveIdRef.current = 0;
  }, []);

  const syncTiles = useCallback(() => {
    if (!gameRef.current) return;

    const nextTiles = readBoard(gameRef.current, rows, cols);
    const correctTileVector = gameRef.current.getCorrectTiles?.();
    const nextCorrectTiles = correctTileVector
      ? new Set(vectorToArray(correctTileVector))
      : new Set<number>();

    (correctTileVector as { delete?: () => void })?.delete?.();

    setTiles(nextTiles);
    setCorrectTiles(nextCorrectTiles);
    setSolvable(gameRef.current.isSolvable());
  }, [cols, rows]);

  const logMove = useCallback((direction: number) => {
    setMoves((currentMoves) => [
      ...currentMoves,
      {
        id: ++moveIdRef.current,
        direction: directionLabels[direction] ?? 'U',
      },
    ]);
  }, []);

  const applyPlayerMove = useCallback(
    (direction: number) => {
      if (!gameRef.current || solving) return;

      if (gameRef.current.applyMove(direction)) {
        moveDirectionsRef.current.push(direction);
        logMove(direction);
        syncTiles();
        setRunning(!gameRef.current.isSolved());
      }
    },
    [logMove, solving, syncTiles],
  );

  useEffect(() => {
    if (!wasmModule?.WebGame) return;

    const nextGame = createGame(wasmModule, rows, cols);
    gameRef.current = nextGame;
    setTiles(readBoard(nextGame, rows, cols));

    const correctTileVector = nextGame.getCorrectTiles?.();
    setCorrectTiles(correctTileVector ? new Set(vectorToArray(correctTileVector)) : new Set());
    (correctTileVector as { delete?: () => void })?.delete?.();

    setSolvable(nextGame.isSolvable());
    clearRunState();

    return () => {
      nextGame.delete?.();
      if (gameRef.current === nextGame) {
        gameRef.current = null;
      }
    };
  }, [clearRunState, cols, rows, wasmModule]);

  useEffect(() => {
    if (!running) return;

    const intervalId = window.setInterval(() => {
      setElapsedSeconds((currentSeconds) => currentSeconds + 1);
    }, 1000);

    return () => window.clearInterval(intervalId);
  }, [running]);

  useEffect(() => {
    const keyDirections: Record<string, number> = {
      ArrowUp: -2,
      w: -2,
      W: -2,
      ArrowRight: -1,
      d: -1,
      D: -1,
      ArrowLeft: 1,
      a: 1,
      A: 1,
      ArrowDown: 2,
      s: 2,
      S: 2,
    };

    const handleKeyDown = (event: KeyboardEvent) => {
      if (event.altKey || event.ctrlKey || event.metaKey || event.shiftKey || event.repeat) return;

      const target = event.target as HTMLElement | null;
      const isEditableTarget =
        target?.isContentEditable ||
        ['INPUT', 'SELECT', 'TEXTAREA'].includes(target?.tagName ?? '');

      if (isEditableTarget) return;

      const direction = keyDirections[event.key];
      if (direction === undefined) return;

      event.preventDefault();
      applyPlayerMove(direction);
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [applyPlayerMove]);

  const handleShuffle = () => {
    if (!gameRef.current || solving) return;

    gameRef.current.shuffle(shuffleMoves);
    syncTiles();
    clearRunState();
    setRunning(true);
  };

  const handleReset = () => {
    if (!gameRef.current || solving) return;

    [...moveDirectionsRef.current].reverse().forEach((direction) => {
      gameRef.current?.applyMove(-direction);
    });
    syncTiles();
    clearRunState();
  };

  const handleTileClick = (index: number) => {
    if (!gameRef.current?.moveTileAt || solving || tiles[index] === null) return;

    const direction = gameRef.current.moveTileAt(index % cols, Math.floor(index / cols));

    if (direction !== 0) {
      moveDirectionsRef.current.push(direction);
      logMove(direction);
      syncTiles();
      setRunning(!gameRef.current.isSolved());
    }
  };

  const handleApplyBoardState = (values: number[]) => {
    if (!gameRef.current?.setGrid || !wasmModule || solving) return false;

    const grid = createVectorInt(wasmModule, values);

    try {
      const applied = gameRef.current.setGrid(grid);
      if (!applied) return false;

      syncTiles();
      clearRunState();
      return true;
    } finally {
      grid.delete?.();
    }
  };

  const getSolution = () => {
    if (!gameRef.current || !wasmModule) return [];

    if (solver === 'ida') {
      try {
        const idaSolution = idaSolverRef.current.solve(gameRef.current, wasmModule, rows, cols);
        if (idaSolution) return idaSolution;
      } catch (error) {
        console.error('IDA* solver failed, falling back to Fringe solver.', error);
      }
    }

    return fringeSolverRef.current.solve(gameRef.current);
  };

  const handleSolve = async () => {
    if (!gameRef.current || solving || !solvable) return;

    setSolving(true);
    setRunning(true);

    try {
      const solution = getSolution();

      for (const direction of solution) {
        if (!gameRef.current) break;

        if (gameRef.current.applyMove(direction)) {
          moveDirectionsRef.current.push(direction);
          logMove(direction);
          syncTiles();
          await sleep(solveStepDelay);
        }
      }

      if (gameRef.current?.isSolved()) {
        setRunning(false);
      }
    } finally {
      setSolving(false);
    }
  };

  const sidebar = (
    <GameSidebar
      tiles={tiles}
      rows={rows}
      cols={cols}
      moves={moves}
      solver={solver}
      solvable={solvable}
      solving={solving}
      idaAvailable={idaAvailable}
      resetDisabled={!gameRef.current || solving}
      boardStateDisabled={!gameRef.current?.setGrid}
      onClose={() => setSidebarOpen(false)}
      onReset={handleReset}
      onSolverChange={setSolver}
      onApplyBoardState={handleApplyBoardState}
    />
  );

  return (
    <main className="h-dvh overflow-hidden bg-gray-50 text-gray-950 transition-colors dark:bg-gray-900 dark:text-gray-50">
      <div className="flex h-full min-h-0">
        <div className="hidden lg:block">{sidebarOpen && sidebar}</div>

        {sidebarOpen && (
          <div
            role="presentation"
            onClick={() => setSidebarOpen(false)}
            className="fixed inset-0 z-40 bg-gray-950/70 lg:hidden"
          >
            <div
              role="presentation"
              onClick={(event) => event.stopPropagation()}
              className="relative h-full"
            >
              {sidebar}
            </div>
          </div>
        )}

        <section className="flex h-full min-h-0 flex-1 flex-col">
          <GameHeader
            elapsedSeconds={elapsedSeconds}
            movesCount={moves.length}
            sidebarOpen={sidebarOpen}
            shuffleDisabled={!gameRef.current || wasmLoading || solving}
            onOpenSidebar={() => setSidebarOpen(true)}
            onShuffle={handleShuffle}
          />

          <div className="flex min-h-0 flex-1 items-center justify-center px-2 py-2 sm:px-3 lg:px-4">
            <div className="flex h-full min-h-0 w-full flex-col justify-center gap-2">
              <PuzzleContainer
                tiles={tiles}
                rows={rows}
                cols={cols}
                correctTiles={correctTiles}
                onTileClick={handleTileClick}
              />

              <GameControls
                rows={rows}
                cols={cols}
                solving={solving}
                solveDisabled={!gameRef.current || wasmLoading || solving || !solvable}
                onRowsChange={(value) => setRows(clampDimension(value))}
                onColsChange={(value) => setCols(clampDimension(value))}
                onSolve={handleSolve}
              />

              {(wasmLoading || wasmError) && (
                <div className="rounded-lg border border-gray-200 bg-white p-4 text-sm text-gray-600 dark:border-gray-800 dark:bg-gray-800 dark:text-gray-300">
                  {wasmLoading ? 'Loading WASM solver...' : `WASM error: ${wasmError}`}
                </div>
              )}
            </div>
          </div>
        </section>
      </div>
    </main>
  );
}
