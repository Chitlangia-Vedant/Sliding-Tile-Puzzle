import { PuzzleTile } from './PuzzleTile';

interface PuzzleContainerProps {
  tiles: (number | null)[];
  rows: number;
  cols: number;
  correctTiles?: Set<number>;
  onTileClick?: (index: number) => void;
}

export function PuzzleContainer({
  tiles,
  rows,
  cols,
  correctTiles = new Set(),
  onTileClick,
}: PuzzleContainerProps) {
  const density = Math.max(rows, cols);
  const gapClass = density <= 4 ? 'gap-2' : density <= 7 ? 'gap-1.5' : 'gap-1';
  const boardMaxHeight = 'calc(100dvh - 9rem)';

  return (
    <div className="mx-auto flex h-full min-h-0 w-full max-w-full items-center justify-center overflow-hidden">
      <div
        className={`grid rounded-lg bg-gray-50 p-1.5 dark:bg-gray-900 sm:p-2 ${gapClass}`}
        style={{
          aspectRatio: `${cols} / ${rows}`,
          maxHeight: boardMaxHeight,
          width: `min(100%, calc(${boardMaxHeight} * ${cols} / ${rows}))`,
          gridTemplateColumns: `repeat(${cols}, minmax(0, 1fr))`,
          gridTemplateRows: `repeat(${rows}, minmax(0, 1fr))`,
        }}
      >
        {tiles.map((tile, index) => (
          <PuzzleTile
            key={index}
            number={tile}
            isCorrect={tile !== null && correctTiles.has(tile)}
            onClick={() => onTileClick?.(index)}
            dimension={density}
          />
        ))}
      </div>
    </div>
  );
}
