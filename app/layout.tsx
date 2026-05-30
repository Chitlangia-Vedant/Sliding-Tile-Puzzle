import type { Metadata } from 'next';
import './globals.css';

export const metadata: Metadata = {
  title: 'Slido - Sliding Tile Puzzle Game',
  description: 'Interactive sliding tile puzzle game with AI solver',
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en" className="dark" suppressHydrationWarning>
      <head>
        <script
          dangerouslySetInnerHTML={{
            __html: `
              try {
                const theme = localStorage.getItem('slido-theme') || 'dark';
                document.documentElement.classList.toggle('dark', theme === 'dark');
              } catch (_) {
                document.documentElement.classList.add('dark');
              }
            `,
          }}
        />
      </head>
      <body className="bg-gray-50 dark:bg-gray-950 text-gray-900 dark:text-gray-50 transition-colors">
        {children}
      </body>
    </html>
  );
}
