interface CounterProps {
  label: string;
  value: string | number;
  variant?: 'time' | 'moves';
}

export function Counter({ label, value, variant = 'moves' }: CounterProps) {
  return (
    <div className="flex min-w-16 flex-col items-center leading-none">
      <span className="mb-1 text-xs text-gray-500 dark:text-gray-400">{label}</span>
      <span className="font-mono text-base font-semibold tabular-nums text-gray-950 dark:text-gray-100 sm:text-lg">
        {value}
      </span>
    </div>
  );
}
