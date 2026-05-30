# Slido - Sliding Tile Puzzle Game

Interactive sliding tile puzzle game built with React/Next.js and integrated with C++ solver via WebAssembly.

## Project Structure

```
├── app/
│   ├── layout.tsx          # Root layout
│   ├── page.tsx            # Main puzzle game interface
│   └── globals.css         # Global styles and Tailwind directives
├── components/
│   ├── PuzzleButton.tsx    # Reusable button component
│   ├── PuzzleTile.tsx      # Individual puzzle tile
│   ├── PuzzleContainer.tsx # Grid layout for tiles
│   ├── Counter.tsx         # Time/Moves display
│   ├── MovesLog.tsx        # Recent moves log
│   ├── BoardStateDisplay.tsx # Board state code block
│   └── SidebarSection.tsx  # Sidebar section wrapper
├── hooks/
│   └── useWasm.ts          # WASM loader hook
├── public/
│   ├── game.js             # Emscripten-generated JS
│   ├── game.wasm           # Compiled WebAssembly module
│   └── game.data           # WASM data file
├── package.json
├── next.config.js
├── tailwind.config.js
└── tsconfig.json
```

## Setup

### 1. Install Dependencies

```bash
npm install
```

### 2. Start Development Server

```bash
npm run dev
```

The app will be available at `http://localhost:3000`

### 3. Build for Production

```bash
npm run build
npm start
```

## Integration with C++ Solver

Your WASM module is already in the `public/` folder (`game.js`, `game.wasm`, `game.data`).

### Loading the WASM Module

The `useWasm` hook in `hooks/useWasm.ts` handles loading the Emscripten-generated module:

```typescript
const { module: wasmModule, loading, error } = useWasm('/game.js');
```

### Calling WASM Functions

Once the module is loaded, you can call your C++ functions exposed via Emscripten:

```typescript
const handleSolve = () => {
  if (!wasmModule) return;
  
  // Call your C++ solver function
  // Example: const solution = wasmModule.solve_puzzle(tiles, rows, cols);
  
  console.log('WASM module:', wasmModule);
};
```

### Common Emscripten Functions

If you used `emscripten_bind` in your C++ code, your functions are available as:

```typescript
wasmModule.solve_puzzle(...)
wasmModule.is_solvable(...)
wasmModule.get_solution(...)
// etc.
```

If you used `ccall` or `cwrap`, use:

```typescript
const result = wasmModule.ccall('function_name', 'return_type', 
  ['param_type1', 'param_type2'], 
  [param1, param2]);
```

## Features

✅ **Interactive UI** - Responsive puzzle game with dark/light mode  
✅ **WASM Integration** - Ready to connect C++ solver functions  
✅ **Component-Based** - Modular, reusable components  
✅ **Tailwind CSS** - Modern styling with design tokens  
✅ **Dark Mode** - Full theme support  
✅ **Responsive** - Desktop and mobile friendly  

## Customization

### Add Dark Mode Toggle

Add a theme switch component to `app/page.tsx`:

```typescript
import { ThemeSwitch } from '@/components/ThemeSwitch';

// In your JSX:
<ThemeSwitch />
```

### Modify Colors

Edit `tailwind.config.js` to change the primary/secondary colors:

```javascript
colors: {
  primary: {
    500: '#4ade80', // Change this
    // ...
  },
}
```

### Update Puzzle Size

The puzzle grid size is controlled by `rows` and `cols` state:

```typescript
const [rows, setRows] = useState(3);
const [cols, setCols] = useState(3);
```

## Next Steps

1. **Connect the Solver**: Update `handleSolve()` in `app/page.tsx` to call your WASM functions
2. **Add Tile Logic**: Implement tile swap logic that calls your WASM module
3. **Test Integration**: Verify WASM module loads correctly (check browser console)
4. **Deploy**: Build and deploy the frontend alongside your WASM files

## Debugging WASM

Open browser DevTools and check:
1. Network tab - verify `game.js`, `game.wasm`, `game.data` load
2. Console - check for errors
3. Application tab - inspect loaded WASM module

```typescript
// Add to your page component for debugging:
useEffect(() => {
  if (wasmModule) {
    console.log('WASM Module loaded:', wasmModule);
    console.log('Available functions:', Object.keys(wasmModule));
  }
}, [wasmModule]);
```

## Resources

- [Emscripten Docs](https://emscripten.org/docs/)
- [Next.js Docs](https://nextjs.org/docs)
- [WebAssembly MDN](https://developer.mozilla.org/en-US/docs/WebAssembly)
- [React Hooks](https://react.dev/reference/react)

## License

MIT
