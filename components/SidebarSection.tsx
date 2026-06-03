interface SidebarSectionProps {
  title: string;
  children: React.ReactNode;
  className?: string;
}

export function SidebarSection({ title, children, className = '' }: SidebarSectionProps) {
  return (
    <div className={className}>
      <div className="mb-3 text-sm font-semibold text-gray-500 dark:text-gray-400">
        {title}
      </div>
      {children}
    </div>
  );
}
