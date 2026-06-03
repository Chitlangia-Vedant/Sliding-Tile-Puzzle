/** @type {import("tailwindcss").Config } */
module.exports = {
  content: [
    './app/**/*.{js,ts,jsx,tsx}',
    './components/**/*.{js,ts,jsx,tsx}',
    './pages/**/*.{js,ts,jsx,tsx}',
  ],
  darkMode: ['class'],
  theme: {
    extend: {
      fontFamily: {
        sans: ['system-ui', '-apple-system', 'sans-serif'],
      },
      colors: {
        primary: {
          100: '#dcfce7',
          200: '#bbf7d0',
          300: '#86efac',
          400: '#4ade80',
          500: '#22c55e',
          600: '#16a34a',
          700: '#15803d',
          800: '#166534',
          900: '#14532d',
        },
        secondary: {
          100: '#52525b',
          200: '#3f3f46',
          300: '#27272a',
          400: '#18181b',
          500: '#3f3f46',
          600: '#27272a',
          700: '#18181b',
          800: '#09090b',
          900: '#000000',
        },
      },
      keyframes: {
        'fade-in-down': {
          '0%': { opacity: '0', transform: 'translateY(-30px)' },
          '100%': { opacity: '1', transform: 'translateY(0)' },
        },
      },
      animation: {
        'fade-in-down': 'fade-in-down 0.4s ease-in-out forwards',
      },
    },
  },
  plugins: [
    require('tailwindcss-animate'),
    require('@tailwindcss/forms'),
    require('@tailwindcss/typography'),
  ],
};
