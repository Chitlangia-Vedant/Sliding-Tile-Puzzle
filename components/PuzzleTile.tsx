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
      <div className="min-h-0 min-w-0 rounded-lg bg-gray-50 dark:bg-gray-900" />
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
            ? `border border-transparent bg-primary-500 text-white
               dark:bg-primary-600 dark:text-white`
            : `border border-gray-200 bg-white text-gray-950 hover:bg-gray-100
               dark:border-gray-700 dark:bg-gray-800 dark:text-gray-100 dark:hover:bg-gray-700`
        }
      `}
    >
      {number}
    </button>
  );
}
