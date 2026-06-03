import { MenuIcon, ShuffleIcon } from 'lucide-react';
import type { CSSProperties } from 'react';

import { Counter } from '@/components/Counter';
import { PuzzleButton } from '@/components/PuzzleButton';
import { ThemeSwitch } from '@/components/ThemeSwitch';
import { formatTime } from '@/lib/puzzleBoard';

interface GameHeaderProps {
  elapsedSeconds: number;
  movesCount: number;
  sidebarOpen: boolean;
  shuffleDisabled: boolean;
  frameStyle?: CSSProperties;
  onOpenSidebar: () => void;
  onShuffle: () => void;
}

export function GameHeader({
  elapsedSeconds,
  movesCount,
  sidebarOpen,
  shuffleDisabled,
  frameStyle,
  onOpenSidebar,
  onShuffle,
}: GameHeaderProps) {
  return (
    <header className="flex shrink-0 justify-center bg-gray-50 px-2 py-2 dark:bg-gray-900 sm:px-3 lg:px-4">
      <div
        className="flex w-full flex-wrap items-center justify-between gap-2 sm:flex-nowrap"
        style={frameStyle}
      >
        <div className="flex items-center gap-2">
          <button
            type="button"
            aria-label={sidebarOpen ? 'Close settings' : 'Open settings'}
            onClick={onOpenSidebar}
            className="inline-flex h-9 w-9 items-center justify-center rounded-lg text-gray-600 transition-colors hover:bg-gray-100 dark:text-gray-200 dark:hover:bg-gray-800"
          >
            <MenuIcon className="h-5 w-5" />
          </button>
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
      </div>
    </header>
  );
}
