// FloatingInput.tsx
'use client';

import { Search } from "lucide-react";

interface Props {
  value: string;
  onChange: (val: string) => void;
  onSubmit: () => void;
  autoFocus?: boolean;
  placeholder?: string;
}

const FloatingInput = ({ value, onChange, onSubmit, autoFocus, placeholder }: Props) => {
  return (
    <form
      onSubmit={(e) => { e.preventDefault(); onSubmit(); }}
      className="w-full max-w-[600px]"
    >
      <div className="flex items-center w-full bg-gray-900 text-white rounded-full shadow-lg px-4 py-2">
        <input
          type="text"
          value={value}
          onChange={(e) => onChange(e.target.value)}
          placeholder={placeholder || ""}
          className="flex-1 bg-transparent outline-none px-2 text-lg"
          autoFocus={autoFocus}
        />
        <Search className="h-5 w-5 text-gray-400" />
      </div>
    </form>
  );
};

export default FloatingInput;
