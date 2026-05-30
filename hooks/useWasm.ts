import { useEffect, useRef, useState } from 'react';

export interface WasmModule {
  memory?: WebAssembly.Memory;
  [key: string]: any;
}

declare global {
  interface Window {
    createGameModule?: (options?: Record<string, unknown>) => Promise<WasmModule>;
    Module?: WasmModule;
  }
}

export function useWasm(wasmPath: string) {
  const [module, setModule] = useState<WasmModule | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const loadedRef = useRef(false);

  useEffect(() => {
    if (loadedRef.current) return;
    loadedRef.current = true;

    const loadWasm = async () => {
      try {
        setLoading(true);

        if (wasmPath.includes('game')) {
          const script = document.createElement('script');
          script.src = wasmPath.replace('.wasm', '.js');
          script.async = true;

          script.onload = async () => {
            try {
              const createModule = window.createGameModule;
              const mod = createModule
                ? await createModule({
                    locateFile: (path: string) => `/${path}`,
                  })
                : window.Module;

              if (!mod) {
                setError('WASM module not loaded');
                return;
              }

              window.Module = mod;
              setModule(mod);
              setError(null);
            } catch (err) {
              setError(err instanceof Error ? err.message : 'Failed to initialize WASM');
            } finally {
              setLoading(false);
            }
          };

          script.onerror = () => {
            setError(`Failed to load ${wasmPath}`);
            setLoading(false);
          };

          document.body.appendChild(script);
        } else {
          // Standard WASM loading
          const response = await fetch(wasmPath);
          const buffer = await response.arrayBuffer();
          const wasmModule = await WebAssembly.instantiate(buffer);
          setModule(wasmModule.instance as WasmModule);
          setError(null);
          setLoading(false);
        }
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Failed to load WASM');
        setLoading(false);
      }
    };

    loadWasm();
  }, [wasmPath]);

  return { module, loading, error };
}
