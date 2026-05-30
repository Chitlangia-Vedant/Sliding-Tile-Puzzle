'use client';

import { MoonIcon, SunIcon } from 'lucide-react';
import { useEffect, useState } from 'react';

type Theme = 'dark' | 'light';

const storageKey = 'slido-theme';

const applyTheme = ({ theme }: { theme: Theme }) => {
  document.documentElement.classList.toggle('dark', theme === 'dark');
  document.documentElement.style.colorScheme = theme;
};

const getStoredTheme = () => {
  const storedTheme = window.localStorage.getItem(storageKey);

  if (storedTheme === 'light' || storedTheme === 'dark') {
    return storedTheme;
  }

  return 'dark';
};

export function ThemeSwitch() {
  const [theme, setTheme] = useState<Theme>('dark');
  const [hasMounted, setHasMounted] = useState(false);

  useEffect(() => {
    const storedTheme = getStoredTheme();
    setTheme(storedTheme);
    applyTheme({ theme: storedTheme });
    setHasMounted(true);
  }, []);

  const handleToggle = () => {
    const nextTheme = theme === 'dark' ? 'light' : 'dark';
    setTheme(nextTheme);
    window.localStorage.setItem(storageKey, nextTheme);
    applyTheme({ theme: nextTheme });
  };

  const Icon = theme === 'dark' ? SunIcon : MoonIcon;
  const label = theme === 'dark' ? 'Switch to light mode' : 'Switch to dark mode';

  return (
    <button
      type="button"
      onClick={handleToggle}
      aria-label={label}
      title={label}
      className="flex h-10 w-10 items-center justify-center rounded-lg border border-gray-300 bg-white text-gray-800 transition-colors hover:bg-gray-100 dark:border-gray-700 dark:bg-gray-900 dark:text-gray-100 dark:hover:bg-gray-800"
    >
      {hasMounted && <Icon className="h-5 w-5" />}
    </button>
  );
}
