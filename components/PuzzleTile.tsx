interface PuzzleTileProps {
  number: number | null;
  isCorrect?: boolean;
  onClick?: () => void;
  disabled?: boolean;
  dimension?: number;
}

export function PuzzleTile({
  number,
  isCorrect = false,
  onClick,
  disabled = false,
  dimension = 4,
}: PuzzleTileProps) {
  const textSize =
    dimension <= 3
      ? 'text-6xl'
      : dimension <= 4
        ? 'text-5xl'
        : dimension <= 6
          ? 'text-3xl'
          : dimension <= 8
            ? 'text-2xl'
            : 'text-lg';

  if (number === null) {
    return (
      <div className="min-h-0 min-w-0 rounded-lg bg-gray-100 shadow-inner dark:bg-gray-950" />
    );
  }

  return (
    <button
      onClick={onClick}
      disabled={disabled}
      className={`
        flex min-h-0 min-w-0 items-center justify-center rounded-lg
        font-bold tabular-nums transition-all duration-300 ease-in-out ${textSize}
        ${
          isCorrect
            ? `border-2 border-primary-500 bg-white text-primary-500 glow-tile
               dark:border-primary-500 dark:bg-gray-800 dark:text-primary-500`
            : `border border-gray-300 bg-white text-gray-950 hover:bg-gray-100
               dark:border-gray-600 dark:bg-gray-800 dark:text-gray-100 dark:hover:bg-gray-700`
        }
      `}
    >
      {number}
    </button>
  );
}
