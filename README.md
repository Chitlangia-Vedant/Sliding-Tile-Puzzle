# Sliding-Tile-Puzzle

> Interactive sliding tile puzzle game with a C++ WebAssembly solver.

![GitHub stars](https://img.shields.io/github/stars/Chitlangia-Vedant/Sliding-Tile-Puzzle?style=for-the-badge&logo=github) ![GitHub forks](https://img.shields.io/github/forks/Chitlangia-Vedant/Sliding-Tile-Puzzle?style=for-the-badge&logo=github) ![GitHub issues](https://img.shields.io/github/issues/Chitlangia-Vedant/Sliding-Tile-Puzzle?style=for-the-badge&logo=github) ![Last commit](https://img.shields.io/github/last-commit/Chitlangia-Vedant/Sliding-Tile-Puzzle?style=for-the-badge&logo=github) ![Next.js](https://img.shields.io/badge/Next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white) ![Tailwind CSS](https://img.shields.io/badge/Tailwind%20CSS-06B6D4?style=for-the-badge&logo=tailwindcss&logoColor=white) ![TypeScript](https://img.shields.io/badge/TypeScript-3178C6?style=for-the-badge&logo=typescript&logoColor=white)

## 📑 Table of Contents

- [Description](#description)
- [Key Features](#key-features)
- [Use Cases](#use-cases)
- [Tech Stack](#tech-stack)
- [Quick Start](#quick-start)
- [Environment Variables](#environment-variables)
- [Key Dependencies](#key-dependencies)
- [Available Scripts](#available-scripts)
- [Project Structure](#project-structure)
- [Development Setup](#development-setup)
- [Contributing](#contributing)

## 📝 Description

Slido is an interactive sliding tile puzzle game designed to demonstrate high-performance client-side computation in web browsers. Built with Next.js, TypeScript, and Tailwind CSS, the application provides a sleek user interface for manipulating puzzle grids, tracking moves, logging sequential steps, and displaying board states dynamically.

## ✨ Key Features

- **⚙️ High-Performance C++ Solver** — Executes complex puzzle-solving algorithms directly in the browser by loading a compiled WebAssembly solver module.
- **🪝 React Hook WASM Loader** — Uses a custom useWasm React hook to asynchronously load and initialize Emscripten-generated JS and WASM assets.
- **🔲 Interactive Grid Layout** — Renders customizable grid tiles, manages real-time moves tracking, and runs timed games via dedicated UI components.
- **🌙 Persistent Dark Mode** — Includes a pre-rendered, local-storage-synchronized dark theme toggle directly embedded in the root document layout.
- **🛠️ Integrated C++ Compilation** — Provides a system Makefile alongside standard npm scripts to easily compile and clean native solver binaries.

## 🎯 Use Cases

- Integrating and executing Emscripten-compiled C++ logic inside a modern Next.js frontend application.
- Studying client-side pathfinding algorithms like sliding tile puzzles using a high-performance WebAssembly module.
- Building and testing responsive tile grid layouts with reactive keyboard or click controls in React.

## 🛠️ Tech Stack

- ▲ **Next.js**
- 🌬️ **Tailwind CSS**
- 📘 **TypeScript**

## ⚡ Quick Start

```bash

# 1. Clone the repository
git clone https://github.com/Chitlangia-Vedant/Sliding-Tile-Puzzle.git

# 2. Install dependencies
npm install

# 3. Configure environment
cp .env.example .env   # then fill in the values

# 4. Start the dev server
npm run dev
```

## 🔑 Environment Variables

The following environment variables are required (see `.env.example`):

```bash
NEXT_PUBLIC_WASM_PATH=
NEXT_PUBLIC_API_URL=
```

## 📦 Key Dependencies

```
react: ^18.3.1
react-dom: ^18.3.1
next: ^15.5.18
lucide-react: ^0.447.0
tailwindcss: ^3.4.17
```

## 🚀 Available Scripts

- **dev** — `npm run dev`
- **build** — `npm run build`
- **start** — `npm run start`
- **lint** — `npm run lint`
- **all** — `make all`
- **clean** — `make clean`

## 📁 Project Structure

```
.
├── .env.example
├── Makefile
├── app
│   ├── globals.css
│   ├── layout.tsx
│   └── page.tsx
├── components
│   ├── BoardStateDisplay.tsx
│   ├── Counter.tsx
│   ├── GameControls.tsx
│   ├── GameHeader.tsx
│   ├── GameSidebar.tsx
│   ├── MovesLog.tsx
│   ├── PuzzleButton.tsx
│   ├── PuzzleContainer.tsx
│   ├── PuzzleTile.tsx
│   ├── SidebarSection.tsx
│   ├── SolvedDialog.tsx
│   └── ThemeSwitch.tsx
├── hooks
│   └── useWasm.ts
├── lib
│   ├── puzzleBoard.ts
│   ├── puzzleConfig.ts
│   ├── puzzleTypes.ts
│   └── wasmVectors.ts
├── next-env.d.ts
├── next.config.js
├── package.json
├── postcss.config.js
├── public
│   ├── game.data
│   ├── game.js
│   └── game.wasm
├── solvers
│   ├── FringeSolver.ts
│   └── IdaSolver.ts
├── src
│   ├── IDAsolver
│   │   ├── database
│   │   │   ├── 3-wd.dat
│   │   │   ├── 4-wd.dat
│   │   │   ├── 443-reg
│   │   │   ├── 443-reg-0.dat
│   │   │   ├── 443-reg-1.dat
│   │   │   ├── 443-reg-2.dat
│   │   │   ├── 555-reg
│   │   │   ├── 555-reg-0.dat
│   │   │   ├── 555-reg-1.dat
│   │   │   ├── 555-reg-2.dat
│   │   │   ├── 8-reg
│   │   │   └── 8-reg-0.dat
│   │   ├── include
│   │   │   ├── DisjointDatabase.h
│   │   │   ├── Idastar.h
│   │   │   ├── Pattern.h
│   │   │   ├── SearchNode.h
│   │   │   ├── SearchNodeRect.h
│   │   │   ├── Util.h
│   │   │   └── WalkingDistance.h
│   │   └── src
│   │       ├── DisjointDatabase.cpp
│   │       ├── Idastar.cpp
│   │       ├── Pattern.cpp
│   │       ├── SearchNode.cpp
│   │       ├── SearchNodeRect.cpp
│   │       ├── Util.cpp
│   │       └── WalkingDistance.cpp
│   ├── core
│   │   ├── Board.cpp
│   │   ├── Board.h
│   │   ├── Direction.cpp
│   │   ├── Direction.h
│   │   ├── Logger.h
│   │   ├── Point.cpp
│   │   ├── Point.h
│   │   ├── Random.h
│   │   ├── Tile.cpp
│   │   └── Tile.h
│   ├── solver
│   │   ├── Solver.cpp
│   │   └── Solver.h
│   └── wasm
│       ├── IdaBridge.cpp
│       ├── IdaBridge.h
│       └── WebGame.cpp
├── tailwind.config.js
├── tsconfig.json
└── tsconfig.tsbuildinfo
```

## 🛠️ Development Setup

### Node.js / JavaScript
1. Install Node.js (v18+ recommended)
2. Install dependencies: `npm install` (or `yarn` / `pnpm install` / `bun install`)
3. Start the dev server: see the **Quick Start** above

## 👥 Contributing

Contributions are welcome! Here's the standard flow:

1. **Fork** the repository
2. **Clone** your fork: `git clone https://github.com/Chitlangia-Vedant/Sliding-Tile-Puzzle.git`
3. **Branch**: `git checkout -b feature/your-feature`
4. **Commit**: `git commit -m 'feat: add some feature'`
5. **Push**: `git push origin feature/your-feature`
6. **Open** a pull request

Please follow the existing code style and include tests for new behavior where applicable.

