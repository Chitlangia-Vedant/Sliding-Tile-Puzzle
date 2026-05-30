import { ActivityIcon, MenuIcon, RotateCcwIcon } from 'lucide-react';

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
    <aside className="flex h-full w-80 shrink-0 flex-col gap-6 overflow-y-auto border-r border-gray-200 bg-white p-6 text-gray-950 dark:border-gray-800 dark:bg-gray-950 dark:text-gray-50">
      <div className="flex items-center justify-between gap-3">
        <div>
          <h1 className="text-2xl font-bold tracking-normal">Slido</h1>
          <p className="text-sm text-gray-500 dark:text-gray-400">Sliding tile solver</p>
        </div>
        <button
          type="button"
          aria-label="Close settings"
          onClick={onClose}
          className="inline-flex h-9 w-9 items-center justify-center rounded-lg border border-gray-300 text-gray-700 transition-colors hover:bg-gray-100 dark:border-gray-700 dark:text-gray-100 dark:hover:bg-gray-800"
        >
          <MenuIcon className="h-5 w-5" />
        </button>
      </div>

      <PuzzleButton variant="outline" onClick={onReset} disabled={resetDisabled}>
        <RotateCcwIcon className="h-4 w-4" />
        Reset Board
      </PuzzleButton>

      <SidebarSection title="Solver Algorithm">
        <select
          value={solver}
          onChange={(event) => onSolverChange(event.target.value as SolverId)}
          className="w-full rounded-lg border-gray-300 bg-white text-sm text-gray-950 transition-colors focus:border-primary-500 focus:ring-primary-500 dark:border-gray-700 dark:bg-gray-900 dark:text-gray-100"
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

      <SidebarSection title="Board State">
        <BoardStateDisplay
          tiles={tiles}
          rows={rows}
          cols={cols}
          disabled={boardStateDisabled || solving}
          onApply={onApplyBoardState}
        />
      </SidebarSection>

      <div className="flex items-center justify-between gap-3 rounded-lg border border-gray-200 bg-white p-3 dark:border-gray-700 dark:bg-gray-800">
        <span className="text-xs font-medium uppercase tracking-wide text-gray-500 dark:text-gray-400">
          Is Solvable?
        </span>
        <span className="flex items-center gap-2 text-sm font-semibold">
          <ActivityIcon className="h-4 w-4 text-primary-500" />
          {solvable ? 'Yes' : 'No'}
        </span>
      </div>

      <MovesLog moves={moves} />
    </aside>
  );
}
