interface Move {
  id: number;
  direction: 'U' | 'D' | 'L' | 'R';
}

interface MovesLogProps {
  moves: Move[];
}

export function MovesLog({ moves }: MovesLogProps) {
  const moveText = moves.map((move) => move.direction).join(',');

  return (
    <div className="max-h-48 overflow-y-auto rounded-lg border border-gray-200 bg-white p-3 dark:border-gray-700 dark:bg-gray-800">
      <div className="mb-3 text-xs font-medium text-gray-500 dark:text-gray-400">
        MOVES LOG
      </div>
      <div className="whitespace-normal break-words rounded bg-gray-100 p-2 font-mono text-xs leading-relaxed text-gray-700 dark:bg-gray-950 dark:text-gray-300">
        {moveText || 'No moves yet'}
      </div>
    </div>
  );
}
