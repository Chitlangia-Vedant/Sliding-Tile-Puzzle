import { MenuIcon, ShuffleIcon } from 'lucide-react';

import { Counter } from '@/components/Counter';
import { PuzzleButton } from '@/components/PuzzleButton';
import { ThemeSwitch } from '@/components/ThemeSwitch';
import { formatTime } from '@/lib/puzzleBoard';

interface GameHeaderProps {
  elapsedSeconds: number;
  movesCount: number;
  sidebarOpen: boolean;
  shuffleDisabled: boolean;
  onOpenSidebar: () => void;
  onShuffle: () => void;
}

export function GameHeader({
  elapsedSeconds,
  movesCount,
  sidebarOpen,
  shuffleDisabled,
  onOpenSidebar,
  onShuffle,
}: GameHeaderProps) {
  return (
    <header className="flex shrink-0 flex-wrap items-center justify-between gap-2 border-b border-gray-200 bg-gray-50 px-2 py-2 dark:border-gray-800 dark:bg-gray-900 sm:flex-nowrap sm:px-4">
      <div className="flex items-center gap-2">
        {!sidebarOpen && (
          <button
            type="button"
            aria-label="Open settings"
            onClick={onOpenSidebar}
            className="inline-flex h-9 w-9 items-center justify-center rounded-lg border border-gray-300 text-gray-700 transition-colors hover:bg-gray-100 dark:border-gray-700 dark:text-gray-100 dark:hover:bg-gray-800"
          >
            <MenuIcon className="h-5 w-5" />
          </button>
        )}
        <PuzzleButton
          variant="outline"
          onClick={onShuffle}
          disabled={shuffleDisabled}
          className="px-3 py-2 text-sm"
        >
          <ShuffleIcon className="h-4 w-4" />
          Shuffle
        </PuzzleButton>
      </div>

      <div className="flex items-center gap-2 sm:gap-4">
        <Counter label="Time" value={formatTime(elapsedSeconds)} />
        <Counter label="Moves" value={movesCount} />
        <ThemeSwitch />
      </div>
    </header>
  );
}
