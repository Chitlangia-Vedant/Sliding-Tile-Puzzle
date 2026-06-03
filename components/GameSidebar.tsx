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
      <div className="flex shrink-0 items-start justify-between gap-3 px-2 py-2">
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
      </div>
    </aside>
  );
}
