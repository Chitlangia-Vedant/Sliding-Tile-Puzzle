<div align="center">
  <img src="assets/Logo.png" alt="Sliding Tile Puzzle Logo" width="120" />

  <h1>Sliding Tile Puzzle </h1>

  <p>
    <strong>A high-performance sliding tile puzzle web app featuring an intelligent auto-solver.</strong><br>
    <em>Pushing the limits of the LearnCPP Chapter 21 project by combining a Next.js frontend with a C++ search engine compiled to WebAssembly.</em>
  </p>

  <a href="https://slidingtilepuzzle.vercel.app/">
    <img src="https://img.shields.io/badge/Live_Demo-Vercel-black?style=for-the-badge&logo=vercel" alt="Live Demo" />
  </a>
  <a href="https://nextjs.org/">
    <img src="https://img.shields.io/badge/Next.js-black?style=for-the-badge&logo=next.js" alt="Next.js" />
  </a>
  <a href="https://isocpp.org/">
    <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B" alt="C++" />
  </a>
  <a href="https://webassembly.org/">
    <img src="https://img.shields.io/badge/WebAssembly-654FF0?style=for-the-badge&logo=webassembly&logoColor=white" alt="WebAssembly" />
  </a>
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License" />
  </a>
</div>

<br />

## The Proof

Watch the engine compute the optimal path and animate the solution directly in the browser.

<div align="center">
  <img src="assets/Solver.gif" alt="Auto-Solver Demo" width="600" />
</div>
<br />
<div align="center">
  <img src="assets/LightMode.png" alt="Light Mode Screenshot" width="400" />
  <img src="assets/DarkMode.png" alt="Dark Mode Screenshot" width="400" />
</div>

## The Problem

The standard 15-puzzle has **10,461,394,944,000** solvable states. Calculating optimal paths through this state space using standard JavaScript causes severe UI blocking and browser timeouts. 

This project solves the computational bottleneck by offloading advanced heuristic algorithms (IDA* with disjoint pattern databases) to a **C++ engine compiled to WebAssembly**, enabling instantaneous, non-blocking puzzle solving within a modern React application.

## Core Features

* **Dynamic Grid Sizes:** Generate square or rectangular boards from `2x2` up to `10x10`.
* **Custom Board State Validation:** Input custom puzzle states manually. The engine calculates parity to determine if the board is mathematically solvable before attempting a search.
* **Intelligent Auto-Solver:** Executes advanced heuristic searches (IDA* and Fringe) to find optimal solution paths.
* **Move Logging:** Tracks and logs every manual user move and automated solver step.

---

## Technical Architecture & Bounds

### Data Flow

```mermaid
graph TD
    subgraph "Frontend (Next.js / React)"
        A[Next.js App `app/page.tsx`] --> B[Puzzle Container / UI]
        B --> C[Components `BoardStateDisplay`, `GameControls`]
        B --> D[State Management `hooks/useWasm.ts`]
    end

    subgraph "TypeScript Bridge"
        D --> E[TS Solvers `solvers/IdaSolver.ts`]
        D --> L[TS Solvers `solvers/FringeSolver.ts`]
        E --> F[Emscripten Loader `public/game.js`]
        L --> F
    end

    subgraph "WebAssembly (WASM)"
        F --> G{WASM Module `public/game.wasm`}
        G --> H[C++ Bridge `src/wasm/WebGame.cpp`, `src/wasm/IdaBridge.cpp`]
    end

    subgraph "C++ Core & Solver"
        H --> I[Core Logic `src/core/Board.cpp`]
        H --> J[IDA* Engine `src/IDAsolver/src/Idastar.cpp`]
        J --> K[(Pattern Databases `public/game.data`)]
        H --> M[Fringe Solver `src/solver/Solver.cpp`]
    end

    %% Styling
    classDef react fill:#0ea5e9,stroke:#0284c7,stroke-width:2px,color:#fff
    classDef ts fill:#3178c6,stroke:#1d4ed8,stroke-width:2px,color:#fff
    classDef wasm fill:#654ff0,stroke:#4338ca,stroke-width:2px,color:#fff
    classDef cpp fill:#f59e0b,stroke:#d97706,stroke-width:2px,color:#fff

    class A,B,C,D react
    class E,F,L ts
    class G,H wasm
    class I,J,K,M cpp
```

### Algorithm Matrix & System Bounds

To respect browser memory constraints, the system dynamically selects the most appropriate solver algorithm based on the board dimensions.

| Grid Size | Recommended Solver | Memory/Payload Impact | Rationale |
| :--- | :--- | :--- | :--- |
| <**4x4** | IDA* + Disjoint Pattern DB | ~2.85 MB (`game.data`) | The 15-puzzle state space requires pre-computed disjoint pattern databases (`.dat` files) for optimal solutions within acceptable timeframes. |
| **≥ 5x5** | Fringe Search | Minimal | The mathematical explosion of a 5x5+ grid exceeds browser memory for pattern databases. The system safely falls back to a low-memory Fringe Search. |

> **Payload Profile:** The core Wasm execution module (`game.wasm`) is incredibly lightweight at **~221.5 KB**. The pattern databases (`game.data`) weigh **~2.85 MB**, which are asynchronously loaded by the Emscripten runtime.

### Clean Developer Experience

The complexity of the C++ bridge is completely abstracted from the React layer using a custom `useWasm` hook:

```typescript
// Example usage within a React component
const { solve, isSolving } = useWasm();

const handleSolveClick = async (boardState) => {
  // Passes the React state to C++, receives an array of optimal moves
  const optimalMoves = await solve(boardState); 
  animateTiles(optimalMoves);
};
```

---

## Quick Start & Local Development

The setup is separated into frontend execution and backend Wasm compilation to accommodate both UI developers and systems engineers.

### Prerequisites
* **[Node.js](https://nodejs.org/)** (v18+ recommended)
* **[Emscripten `v5.0.7`](https://emscripten.org/)** (Required *only* if you intend to modify the C++ source files).

### 1. Frontend Setup

Clone the repository and install dependencies:
```bash
git clone [https://github.com/Chitlangia-Vedant/Sliding-Tile-Puzzle.git](https://github.com/Chitlangia-Vedant/Sliding-Tile-Puzzle.git)
cd Sliding-Tile-Puzzle
npm install
```

### 2. Configuration
Copy the template environment file:
```bash
cp .env.example .env.local
```
This sets standard paths like `NEXT_PUBLIC_WASM_PATH=/game.js`. No external API keys are required.

### 3. Start the Development Server
```bash
npm run dev
```
Navigate to `http://localhost:3000`.

<details>
<summary><b>C++ / WebAssembly Compilation (Advanced)</b></summary>

If you modify the algorithms in `src/IDAsolver` or `src/core`, you must recompile the WebAssembly binaries.

1. Ensure Emscripten `v5.0.7` is installed and activated (`emsdk activate 5.0.7`).
2. Run the included Makefile from the project root:
   ```bash
   make
   ```
This updates `public/game.wasm`, `public/game.js`, and packages the `.dat` files into `public/game.data`.
</details>

---

## Project Structure

```text
Sliding-Tile-Puzzle/
├── app/                  # Next.js App Router (Pages, Layouts, CSS)
├── components/           # React UI Components (Board, Controls, Logs)
├── hooks/                # Custom React Hooks (useWasm.ts)
├── lib/                  # TypeScript Types & Configurations
├── public/               # Static Assets & Compiled Wasm (game.js, game.wasm, game.data)
├── solvers/              # TypeScript bridges to the Wasm Engine
└── src/                  # C++ Core Engine
    ├── core/             # Base Board and Tile Logic
    ├── IDAsolver/        # IDA* Search Algorithm & Pattern Databases
    └── wasm/             # Emscripten Bindings (IdaBridge.cpp)
```

---

## Roadmap

- [ ] **Global Leaderboards:** Track fastest solve times and lowest move counts.
- [ ] **Hover-on Control:** Implement "no-click" mouse inputs for faster manual solving (inspired by [ske7/15-Puzzle](https://github.com/ske7/15-Puzzle)).

---

## Contributing

While I am not actively recruiting maintainers, pull requests for UI improvements, new heuristic algorithms, or Wasm optimizations are always welcome! 

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## Acknowledgements & References

This project builds upon the research, algorithms, and tutorials of the following creators. *Note: A modified version of the IDA\* + database solver code from Michael Kim is utilized in this implementation.*

* **[LearnCPP Chapter 21](https://www.learncpp.com/cpp-tutorial/chapter-21-project/)** - The original C++ console project that inspired this web port.
* **Michael Kim** - [Solving the 15 Puzzle (Blog)](https://michael.kim/blog/puzzle) | [15puzzle GitHub](https://github.com/MichaelKim/15puzzle)
* **Luke LaValva** - [Theory of Sliding](https://www.lukelavalva.com/theoryofsliding)
* **Vladyslav Cherednichenko (iCherya)** - [Sliding Puzzle Game](https://icherya.github.io/Fifteen-Puzzle/)

---

## License

This project is open-source and available under the [MIT License](LICENSE). Copyright (c) 2026 Vedant Chitlangia.