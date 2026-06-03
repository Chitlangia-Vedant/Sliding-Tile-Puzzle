import {
  ChevronDownIcon,
  ChevronUpIcon,
  PartyPopperIcon,
  RefreshCwIcon,
  XIcon,
} from 'lucide-react';
import { useState } from 'react';

import { PuzzleButton } from '@/components/PuzzleButton';
import { formatTime } from '@/lib/puzzleBoard';
import type { MoveEntry } from '@/lib/puzzleTypes';

interface SolvedDialogProps {
  open: boolean;
  elapsedSeconds: number;
  moves: MoveEntry[];
  initialBoard: (number | null)[];
  cols: number;
  solverUsed: boolean;
  onClose: () => void;
  onNewGame: () => void;
}

const formatBoard = (tiles: (number | null)[], cols: number) => {
  const rows = Array.from({ length: Math.ceil(tiles.length / cols) }, (_, rowIndex) =>
    tiles
      .slice(rowIndex * cols, rowIndex * cols + cols)
      .map((tile) => (tile === null ? '0' : tile))
      .join(', '),
  );

  return `[${rows.join('\n')}]`;
};

export function SolvedDialog({
  open,
  elapsedSeconds,
  moves,
  initialBoard,
  cols,
  solverUsed,
  onClose,
  onNewGame,
}: SolvedDialogProps) {
  const [advancedOpen, setAdvancedOpen] = useState(false);

  if (!open) return null;

  const moveLog = moves.map((move) => move.direction).join(', ');

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-gray-950/70 px-4 py-6">
      <section
        role="dialog"
        aria-modal="true"
        aria-labelledby="solved-dialog-title"
        className="flex max-h-full w-full max-w-md flex-col overflow-hidden rounded-lg bg-white text-gray-950 shadow-2xl dark:bg-gray-950 dark:text-gray-50"
      >
        <div className="flex items-start justify-between gap-3 border-b border-gray-100 p-5 dark:border-gray-900">
          <div className="flex min-w-0 items-start gap-3">
            <div className="flex h-10 w-10 shrink-0 items-center justify-center rounded-lg bg-primary-100 text-primary-700 dark:bg-primary-950 dark:text-primary-300">
              <PartyPopperIcon className="h-5 w-5" />
            </div>
            <div>
              <h2 id="solved-dialog-title" className="text-xl font-semibold tracking-normal">
                Solved!
              </h2>
              <p className="mt-1 text-sm text-gray-500 dark:text-gray-400">
                Nice finish. Here is the run summary.
              </p>
            </div>
          </div>
          <button
            type="button"
            aria-label="Close solved dialog"
            onClick={onClose}
            className="inline-flex h-9 w-9 shrink-0 items-center justify-center rounded-lg text-gray-500 transition-colors hover:bg-gray-100 hover:text-gray-900 dark:text-gray-300 dark:hover:bg-gray-900 dark:hover:text-gray-50"
          >
            <XIcon className="h-5 w-5" />
          </button>
        </div>

        <div className="overflow-y-auto p-5">
          <div className="grid grid-cols-2 gap-3">
            <div className="rounded-lg bg-gray-50 p-3 dark:bg-gray-900">
              <div className="text-xs font-semibold uppercase text-gray-500 dark:text-gray-400">
                Time
              </div>
              <div className="mt-1 font-mono text-2xl font-semibold">{formatTime(elapsedSeconds)}</div>
            </div>
            <div className="rounded-lg bg-gray-50 p-3 dark:bg-gray-900">
              <div className="text-xs font-semibold uppercase text-gray-500 dark:text-gray-400">
                Moves
              </div>
              <div className="mt-1 font-mono text-2xl font-semibold">{moves.length}</div>
            </div>
          </div>

          <button
            type="button"
            onClick={() => setAdvancedOpen((isOpen) => !isOpen)}
            aria-expanded={advancedOpen}
            className="mt-4 flex w-full items-center justify-between rounded-lg px-3 py-2 text-sm font-semibold text-gray-700 transition-colors hover:bg-gray-50 dark:text-gray-100 dark:hover:bg-gray-900"
          >
            <span>Advance</span>
            {advancedOpen ? (
              <ChevronUpIcon className="h-4 w-4" />
            ) : (
              <ChevronDownIcon className="h-4 w-4" />
            )}
          </button>

          {advancedOpen && (
            <div className="mt-3 space-y-3">
              <div>
                <div className="mb-2 text-sm font-semibold text-gray-500 dark:text-gray-400">
                  Initial Board
                </div>
                <pre className="max-h-40 overflow-auto whitespace-pre-wrap rounded-lg bg-gray-50 p-3 font-mono text-xs leading-relaxed text-gray-700 dark:bg-gray-900 dark:text-gray-300">
                  {formatBoard(initialBoard, cols)}
                </pre>
              </div>

              <div>
                <div className="mb-2 text-sm font-semibold text-gray-500 dark:text-gray-400">
                  Move Logs
                </div>
                <div className="max-h-32 overflow-auto whitespace-normal break-words rounded-lg bg-gray-50 p-3 font-mono text-xs leading-relaxed text-gray-700 dark:bg-gray-900 dark:text-gray-300">
                  {moveLog || 'No moves recorded'}
                </div>
              </div>

              <div className="flex items-center justify-between rounded-lg bg-gray-50 p-3 text-sm dark:bg-gray-900">
                <span className="font-semibold text-gray-500 dark:text-gray-400">Solver</span>
                <span className="font-semibold">{solverUsed ? 'Used' : 'Not Used'}</span>
              </div>
            </div>
          )}
        </div>

        <div className="border-t border-gray-100 p-5 dark:border-gray-900">
          <PuzzleButton variant="primary" onClick={onNewGame} className="w-full">
            <RefreshCwIcon className="h-4 w-4" />
            New Game
          </PuzzleButton>
        </div>
      </section>
    </div>
  );
}
