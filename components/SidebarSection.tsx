interface SidebarSectionProps {
  title: string;
  children: React.ReactNode;
  className?: string;
}

export function SidebarSection({ title, children, className = '' }: SidebarSectionProps) {
  return (
    <div className={className}>
      <div className="text-xs text-gray-400 dark:text-gray-400 font-medium mb-3 uppercase tracking-wide">
        {title}
      </div>
      {children}
    </div>
  );
}
