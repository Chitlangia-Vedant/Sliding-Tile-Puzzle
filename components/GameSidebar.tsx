import { MenuIcon, RotateCcwIcon } from 'lucide-react';

import { BoardStateDisplay } from '@/components/BoardStateDisplay';
import { MovesLog } from '@/components/MovesLog';
import { PuzzleButton } from '@/components/PuzzleButton';
import { SidebarSection } from '@/components/SidebarSection';
import { solverOptions } from '@/lib/puzzleConfig';
import type { MoveEntry, SolverId } from '@/lib/puzzleTypes';

interface GameSidebarProps {
  tiles: (number | null)[];
  rows: number;
  cols: number;
  moves: MoveEntry[];
  solver: SolverId;
  solvable: boolean;
  solving: boolean;
  idaAvailable: boolean;
  resetDisabled: boolean;
  boardStateDisabled: boolean;
  onClose: () => void;
  onReset: () => void;
  onSolverChange: (solver: SolverId) => void;
  onApplyBoardState: (values: number[]) => boolean;
}

export function GameSidebar({
  tiles,
  rows,
  cols,
  moves,
  solver,
  solvable,
  solving,
  idaAvailable,
  resetDisabled,
  boardStateDisabled,
  onClose,
  onReset,
  onSolverChange,
  onApplyBoardState,
}: GameSidebarProps) {
  return (
    <aside className="flex h-full w-80 shrink-0 flex-col overflow-y-auto border-r border-gray-100 bg-white text-gray-950 dark:border-gray-900 dark:bg-gray-950 dark:text-gray-50">
      <div className="flex shrink-0 items-start justify-between gap-3 px-2 py-4">
        <div>
          <h1 className="text-2xl font-semibold tracking-normal">Slido</h1>
          <p className="text-sm text-gray-500 dark:text-gray-400">Sliding tile solver</p>
        </div>
        <button
          type="button"
          aria-label="Close settings"
          onClick={onClose}
          className="inline-flex h-9 w-9 items-center justify-center rounded-lg text-gray-600 transition-colors hover:bg-gray-100 dark:text-gray-200 dark:hover:bg-gray-900"
        >
          <MenuIcon className="h-5 w-5" />
        </button>
      </div>

      <div className="flex flex-col gap-6 px-6 pb-6 pt-4">
        <PuzzleButton variant="outline" onClick={onReset} disabled={resetDisabled}>
          <RotateCcwIcon className="h-4 w-4" />
          Reset Board
        </PuzzleButton>

        <SidebarSection title="Solver algorithm">
          <select
            value={solver}
            onChange={(event) => onSolverChange(event.target.value as SolverId)}
            className="w-full rounded-lg border-0 bg-gray-50 text-sm text-gray-950 transition-colors focus:ring-2 focus:ring-primary-300 dark:bg-gray-900 dark:text-gray-100"
          >
            {solverOptions.map((option) => (
              <option key={option.id} value={option.id} disabled={option.id === 'ida' && !idaAvailable}>
                {option.label}
              </option>
            ))}
          </select>
          <p className="mt-2 text-xs text-gray-500 dark:text-gray-400">
            IDA* is available for 4x4 and smaller boards.
          </p>
        </SidebarSection>

        <SidebarSection
          title="Board state"
          className="relative"
        >
          <div className={`absolute right-0 top-0 text-sm font-semibold ${solvable ? 'text-emerald-600 dark:text-emerald-400' : 'text-red-600 dark:text-red-400'}`}>
            {solvable ? 'Solvable' : 'Not solvable'}
          </div>
          <BoardStateDisplay
            tiles={tiles}
            rows={rows}
            cols={cols}
            disabled={boardStateDisabled || solving}
            onApply={onApplyBoardState}
          />
        </SidebarSection>

        <MovesLog moves={moves} />
        <div className="mt-auto pt-6 pb-2 border-t border-gray-200 dark:border-gray-800">
          <div className="flex flex-col space-y-4">
            
            {/* GitHub Link */}
            <a
              href="https://github.com/Chitlangia-Vedant/Sliding-Tile-Puzzle"
              target="_blank"
              rel="noopener noreferrer"
              className="flex items-center space-x-3 text-sm font-medium text-gray-600 hover:text-black dark:text-gray-400 dark:hover:text-white transition-colors duration-200 group"
            >
              <svg
                xmlns="http://www.w3.org/2000/svg"
                width="20"
                height="20"
                fill="currentColor"
                viewBox="0 0 24 24"
                className="group-hover:scale-110 transition-transform duration-200"
              >
                <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z" />
              </svg>
              <span>GitHub Repo</span>
            </a>

            {/* LinkedIn Link */}
            <a
              href="https://www.linkedin.com/in/vedant2004"
              target="_blank"
              rel="noopener noreferrer"
              className="flex items-center space-x-3 text-sm font-medium text-gray-600 hover:text-[#0A66C2] dark:text-gray-400 dark:hover:text-[#3b82f6] transition-colors duration-200 group"
            >
              <svg
                xmlns="http://www.w3.org/2000/svg"
                width="20"
                height="20"
                fill="currentColor"
                viewBox="0 0 24 24"
                className="group-hover:scale-110 transition-transform duration-200"
              >
                <path d="M19 0h-14c-2.761 0-5 2.239-5 5v14c0 2.761 2.239 5 5 5h14c2.762 0 5-2.239 5-5v-14c0-2.761-2.238-5-5-5zm-11 19h-3v-11h3v11zm-1.5-12.268c-.966 0-1.75-.79-1.75-1.764s.784-1.764 1.75-1.764 1.75.79 1.75 1.764-.783 1.764-1.75 1.764zm13.5 12.268h-3v-5.604c0-3.368-4-3.113-4 0v5.604h-3v-11h3v1.765c1.396-2.586 7-2.777 7 2.476v6.759z" />
              </svg>
              <span>LinkedIn</span>
            </a>

          </div>
        </div>
      </div>
    </aside>
  );
}
