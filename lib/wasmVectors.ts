import type { WasmModule } from '@/hooks/useWasm';

export const vectorToArray = (value: unknown): number[] => {
  if (!value) return [];
  if (Array.isArray(value)) return value.map(Number);

  const iterable = value as Iterable<number>;
  if (typeof iterable[Symbol.iterator] === 'function') {
    return Array.from(iterable, Number);
  }

  const sized = value as { size?: () => number; get?: (index: number) => number };
  if (typeof sized.size === 'function' && typeof sized.get === 'function') {
    return Array.from({ length: sized.size() }, (_, index) => Number(sized.get?.(index)));
  }

  return [];
};

export const createVectorInt = (module: WasmModule, values: number[]) => {
  const vector = new module.VectorInt();
  values.forEach((value) => vector.push_back(value));
  return vector;
};

export const createVectorVectorInt = (module: WasmModule, patterns: number[][]) => {
  const vector = new module.VectorVectorInt();
  const children = patterns.map((pattern) => createVectorInt(module, pattern));

  children.forEach((child) => vector.push_back(child));

  return {
    vector,
    delete: () => {
      children.forEach((child) => child.delete?.());
      vector.delete?.();
    },
  };
};
