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
    <div className="max-h-48 overflow-y-auto">
      <div className="mb-3 text-sm font-semibold text-gray-500 dark:text-gray-400">
        Moves log
      </div>
      <div className="whitespace-normal break-words rounded-lg bg-gray-50 p-3 font-mono text-xs leading-relaxed text-gray-700 dark:bg-gray-900 dark:text-gray-300">
        {moveText || 'No moves yet'}
      </div>
    </div>
  );
}
