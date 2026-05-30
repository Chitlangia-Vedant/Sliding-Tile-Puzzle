import { SparklesIcon } from 'lucide-react';

import { PuzzleButton } from '@/components/PuzzleButton';
import { maxDimension, minDimension } from '@/lib/puzzleConfig';

interface GameControlsProps {
  rows: number;
  cols: number;
  solving: boolean;
  solveDisabled: boolean;
  onRowsChange: (value: number) => void;
  onColsChange: (value: number) => void;
  onSolve: () => void;
}

export function GameControls({
  rows,
  cols,
  solving,
  solveDisabled,
  onRowsChange,
  onColsChange,
  onSolve,
}: GameControlsProps) {
  return (
    <div className="flex shrink-0 flex-col gap-2 border-t border-gray-200 bg-gray-50 pt-2 dark:border-gray-800 dark:bg-gray-900 sm:flex-row sm:items-end sm:justify-center">
      <div className="grid grid-cols-2 gap-2">
        <label className="text-sm text-gray-500 dark:text-gray-400">
          Row
          <input
            type="number"
            min={minDimension}
            max={maxDimension}
            value={rows}
            onChange={(event) => onRowsChange(Number(event.target.value))}
            disabled={solving}
            className="mt-1 w-24 rounded-lg border-gray-300 bg-white py-1 text-gray-950 focus:border-primary-500 focus:ring-primary-500 dark:border-gray-700 dark:bg-gray-900 dark:text-gray-100"
          />
        </label>
        <label className="text-sm text-gray-500 dark:text-gray-400">
          Column
          <input
            type="number"
            min={minDimension}
            max={maxDimension}
            value={cols}
            onChange={(event) => onColsChange(Number(event.target.value))}
            disabled={solving}
            className="mt-1 w-24 rounded-lg border-gray-300 bg-white py-1 text-gray-950 focus:border-primary-500 focus:ring-primary-500 dark:border-gray-700 dark:bg-gray-900 dark:text-gray-100"
          />
        </label>
      </div>

      <PuzzleButton
        variant="primary"
        onClick={onSolve}
        disabled={solveDisabled}
        className="w-full px-3 py-2 text-sm sm:w-auto"
      >
        <SparklesIcon className="h-4 w-4" />
        {solving ? 'Solving' : 'Solve Puzzle'}
      </PuzzleButton>
    </div>
  );
}
