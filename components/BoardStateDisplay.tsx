'use client';

import { useEffect, useLayoutEffect, useMemo, useRef, useState } from 'react';

interface BoardStateDisplayProps {
  tiles: (number | null)[];
  rows: number;
  cols: number;
  disabled?: boolean;
  onApply: (values: number[]) => boolean;
}

const formatBoardState = (tiles: (number | null)[], cols: number) => {
  const rows = Array.from({ length: Math.ceil(tiles.length / cols) }, (_, rowIndex) =>
    tiles
      .slice(rowIndex * cols, rowIndex * cols + cols)
      .map((tile) => (tile === null ? '0' : tile))
      .join(','),
  );

  return `[${rows.join('\n')}]`;
};

const parseBoardState = (value: string, rows: number, cols: number) => {
  const expectedSize = rows * cols;
  const tokens = value
    .replace(/[\[\]]/g, ' ')
    .split(/[\s,]+/)
    .filter(Boolean);

  if (tokens.length !== expectedSize) {
    return {
      error: `Expected ${expectedSize} values.`,
      values: [],
    };
  }

  const values = tokens.map(Number);

  if (values.some((item) => !Number.isInteger(item))) {
    return {
      error: 'Use integers only.',
      values: [],
    };
  }

  const seen = new Set(values);
  const invalidValue = values.find((item) => item < 0 || item >= expectedSize);

  if (invalidValue !== undefined) {
    return {
      error: `Values must be 0-${expectedSize - 1}.`,
      values: [],
    };
  }

  if (seen.size !== expectedSize) {
    return {
      error: 'Values must not repeat.',
      values: [],
    };
  }

  return {
    error: null,
    values,
  };
};

export function BoardStateDisplay({
  tiles,
  rows,
  cols,
  disabled = false,
  onApply,
}: BoardStateDisplayProps) {
  const formattedBoard = useMemo(() => formatBoardState(tiles, cols), [cols, tiles]);
  const textareaRef = useRef<HTMLTextAreaElement | null>(null);
  const [value, setValue] = useState(formattedBoard);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    setValue(formattedBoard);
    setError(null);
  }, [formattedBoard]);

  useLayoutEffect(() => {
    if (!textareaRef.current) return;

    textareaRef.current.style.height = 'auto';
    textareaRef.current.style.height = `${textareaRef.current.scrollHeight}px`;
  }, [value]);

  const handleApply = () => {
    const parsed = parseBoardState(value, rows, cols);

    if (parsed.error) {
      setError(parsed.error);
      return;
    }

    const applied = onApply(parsed.values);
    setError(applied ? null : 'Backend rejected this board.');
  };

  return (
    <div className="space-y-2">
      <textarea
        ref={textareaRef}
        value={value}
        onChange={(event) => {
          setValue(event.target.value);
          setError(null);
        }}
        disabled={disabled}
        spellCheck={false}
        className="min-h-28 w-full resize-none overflow-hidden rounded-lg border-0 bg-gray-50 p-3 font-mono text-xs text-gray-800 transition-colors focus:ring-2 focus:ring-primary-300 dark:bg-gray-900 dark:text-gray-300"
      />
      <div className="flex items-center justify-between gap-2">
        <span className="min-h-4 text-xs text-red-600 dark:text-red-400">{error}</span>
        <button
          type="button"
          onClick={handleApply}
          disabled={disabled}
          className="rounded-lg px-3 py-1 text-xs font-semibold text-gray-700 transition-colors hover:bg-gray-100 disabled:cursor-not-allowed disabled:opacity-50 dark:text-gray-100 dark:hover:bg-gray-900"
        >
          Apply
        </button>
      </div>
    </div>
  );
}
