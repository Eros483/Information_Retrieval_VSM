/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    './src/pages/**/*.{js,ts,jsx,tsx,mdx}',
    './src/components/**/*.{js,ts,jsx,tsx,mdx}',
    './src/app/**/*.{js,ts,jsx,tsx,mdx}',
  ],
  theme: {
    extend: {
      colors: {
        github: {
          bg: '#0d1117',
          border: '#30363d',
          text: '#e6edf3',
          accent: '#238636',
          blue: '#0969da',
          gray: {
            50: '#f6f8fa',
            100: '#eaeef2',
            200: '#d0d7de',
            300: '#afb8c1',
            400: '#8c959f',
            500: '#6e7681',
            600: '#57606a',
            700: '#424a53',
            800: '#32383f',
            900: '#24292f'
          }
        }
      },
      fontFamily: {
        mono: ['ui-monospace', 'SFMono-Regular', 'Monaco', 'Cascadia Code', 'Roboto Mono', 'Oxygen Mono', 'Ubuntu Monospace', 'Source Code Pro', 'Fira Code', 'Droid Sans Mono', 'Courier New', 'monospace'],
      },
      animation: {
        'fade-in': 'fadeIn 0.5s ease-in-out',
        'slide-up': 'slideUp 0.3s ease-out',
        'pulse-subtle': 'pulse 2s cubic-bezier(0.4, 0, 0.6, 1) infinite',
      },
      keyframes: {
        fadeIn: {
          '0%': { opacity: '0' },
          '100%': { opacity: '1' },
        },
        slideUp: {
          '0%': { transform: 'translateY(10px)', opacity: '0' },
          '100%': { transform: 'translateY(0)', opacity: '1' },
        }
      }
    },
  },
  plugins: [],
  darkMode: 'class',
}