import { MinusIcon, PlusIcon } from 'lucide-react';
import { useEffect, useState, type CSSProperties } from 'react';

import { PuzzleButton } from '@/components/PuzzleButton';
import { maxDimension, minDimension } from '@/lib/puzzleConfig';

interface GameControlsProps {
  rows: number;
  cols: number;
  solving: boolean;
  solveDisabled: boolean;
  frameStyle?: CSSProperties;
  onRowsChange: (value: number) => void;
  onColsChange: (value: number) => void;
  onSolve: () => void;
}

export function GameControls({
  rows,
  cols,
  solving,
  solveDisabled,
  frameStyle,
  onRowsChange,
  onColsChange,
  onSolve,
}: GameControlsProps) {
  const [rowValue, setRowValue] = useState(String(rows));
  const [colValue, setColValue] = useState(String(cols));

  useEffect(() => {
    setRowValue(String(rows));
  }, [rows]);

  useEffect(() => {
    setColValue(String(cols));
  }, [cols]);

  const clampValue = (value: number) => Math.min(maxDimension, Math.max(minDimension, value));

  const applyBoth = (nextRows: number, nextCols: number) => {
    const clampedRows = clampValue(nextRows);
    const clampedCols = clampValue(nextCols);
    onRowsChange(clampedRows);
    onColsChange(clampedCols);
    setRowValue(String(clampedRows));
    setColValue(String(clampedCols));
  };

  const applyRows = (value: number) => {
    const nextValue = clampValue(value);
    onRowsChange(nextValue);
    setRowValue(String(nextValue));
  };

  const applyCols = (value: number) => {
    const nextValue = clampValue(value);
    onColsChange(nextValue);
    setColValue(String(nextValue));
  };

  const handleRowsInputChange = (value: string) => {
    const nextValue = value.replace(/[^\d]/g, '');
    setRowValue(nextValue);

    const numericValue = Number(nextValue);
    if (Number.isInteger(numericValue) && numericValue >= minDimension && numericValue <= maxDimension) {
      onRowsChange(numericValue);
    }
  };

  const handleColsInputChange = (value: string) => {
    const nextValue = value.replace(/[^\d]/g, '');
    setColValue(nextValue);

    const numericValue = Number(nextValue);
    if (Number.isInteger(numericValue) && numericValue >= minDimension && numericValue <= maxDimension) {
      onColsChange(numericValue);
    }
  };

  const handleRowsInputBlur = () => {
    const numericValue = Number(rowValue);
    applyRows(Number.isInteger(numericValue) ? numericValue : rows);
  };

  const handleColsInputBlur = () => {
    const numericValue = Number(colValue);
    applyCols(Number.isInteger(numericValue) ? numericValue : cols);
  };

  return (
    <div className="flex shrink-0 justify-center bg-gray-50 pt-2 dark:bg-gray-900">
      <div
        className="flex w-full flex-col items-center gap-2 sm:flex-row sm:justify-between"
        style={frameStyle}
      >
        <div className="flex items-center gap-2">
          <button
            type="button"
            aria-label="Decrease rows and columns"
            onClick={() => applyBoth(rows - 1, cols - 1)}
            disabled={solving || rows <= minDimension || cols <= minDimension}
            className="inline-flex h-9 w-9 items-center justify-center rounded-lg text-gray-700 transition-colors hover:bg-gray-100 disabled:cursor-not-allowed disabled:opacity-40 dark:text-gray-100 dark:hover:bg-gray-800"
          >
            <MinusIcon className="h-4 w-4" />
          </button>

          <div className="flex h-9 items-center gap-2 rounded-lg bg-white px-3 dark:bg-gray-800">
            <label className="sr-only" htmlFor="rows-input">
              Rows
            </label>
            <input
              id="rows-input"
              type="text"
              inputMode="numeric"
              value={rowValue}
              onChange={(event) => handleRowsInputChange(event.target.value)}
              onBlur={handleRowsInputBlur}
              disabled={solving}
              className="w-8 border-0 bg-transparent p-0 text-center text-gray-950 focus:ring-0 dark:text-gray-100"
            />
            <span className="text-sm text-gray-400">x</span>
            <label className="sr-only" htmlFor="columns-input">
              Columns
            </label>
              <input
                id="columns-input"
                type="text"
                inputMode="numeric"
                value={colValue}
                onChange={(event) => handleColsInputChange(event.target.value)}
                onBlur={handleColsInputBlur}
                disabled={solving}
                className="w-8 border-0 bg-transparent p-0 text-center text-gray-950 focus:ring-0 dark:text-gray-100"
              />
          </div>

          <button
            type="button"
            aria-label="Increase rows and columns"
            onClick={() => applyBoth(rows + 1, cols + 1)}
            disabled={solving || rows >= maxDimension || cols >= maxDimension}
            className="inline-flex h-9 w-9 items-center justify-center rounded-lg text-gray-700 transition-colors hover:bg-gray-100 disabled:cursor-not-allowed disabled:opacity-40 dark:text-gray-100 dark:hover:bg-gray-800"
          >
            <PlusIcon className="h-4 w-4" />
          </button>
        </div>

        <PuzzleButton
          variant="primary"
          onClick={onSolve}
          disabled={solveDisabled}
          className="w-full px-3 py-2 text-sm sm:w-auto"
        >
          {solving ? 'Solving' : 'Solve'}
        </PuzzleButton>
      </div>
    </div>
  );
}
