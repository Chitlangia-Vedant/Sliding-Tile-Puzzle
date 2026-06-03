interface PuzzleButtonProps {
  variant: 'primary' | 'outline';
  onClick?: () => void;
  className?: string;
  children: React.ReactNode;
  disabled?: boolean;
  type?: 'button' | 'submit';
}

export function PuzzleButton({
  variant,
  onClick,
  className = '',
  children,
  disabled = false,
  type = 'button',
}: PuzzleButtonProps) {
  const baseClasses =
    'inline-flex items-center justify-center gap-2 rounded-lg px-4 py-2 text-base font-semibold transition-colors duration-300 ease-in-out disabled:cursor-not-allowed disabled:opacity-50';

  const variantClasses =
    variant === 'primary'
      ? 'bg-primary-500 text-white hover:bg-primary-600 dark:hover:bg-primary-600'
      : 'bg-transparent text-gray-800 hover:bg-gray-100 dark:text-gray-100 dark:hover:bg-gray-900';

  return (
    <button
      type={type}
      onClick={onClick}
      disabled={disabled}
      className={`${baseClasses} ${variantClasses} ${className}`}
    >
      {children}
    </button>
  );
}
