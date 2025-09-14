'use client';

import { FileText, Clock } from "lucide-react";

export interface SearchResult {
  query: string;
  results: [string, number][];
  elapsed_time: number;
}

interface SearchResultsProps {
  results: SearchResult;
  selectedIndex?: number;
}

const SearchResults = ({ results, selectedIndex = -1 }: SearchResultsProps) => {
  const items = results.results || [];

  return (
    <div className="w-full max-w-[600px] bg-white/20 dark:bg-gray-800/30 backdrop-blur-md rounded-lg shadow-lg border border-gray-200/30 dark:border-gray-700/40 p-6 mt-2 transition-all">
      {/* Header */}
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-white/90 dark:text-white">
          Search Results
        </h3>
        <div className="flex items-center space-x-2 text-sm text-white/60 dark:text-gray-400">
          <Clock className="h-4 w-4" />
          <span>{(results.elapsed_time * 1000).toFixed(2)}ms</span>
        </div>
      </div>

      {/* Query */}
      <div className="mb-4">
        <p className="text-sm text-white/70 dark:text-gray-300">
          Query:{' '}
          <span className="font-mono bg-white/10 dark:bg-gray-700/30 px-2 py-1 rounded">
            {results.query}
          </span>
        </p>
      </div>

      {/* Results */}
      {items.length > 0 ? (
        <div className="space-y-3">
          {items.map(([filename, score], index) => (
            <div
              key={index}
              className={`flex items-center justify-between p-3 rounded-lg border transition-all ${
                index === selectedIndex
                  ? "bg-blue-600/80 text-white border-blue-500/80"
                  : "bg-white/10 dark:bg-gray-700/30 border-gray-200/30 dark:border-gray-600/30"
              }`}
            >
              <div className="flex items-center space-x-3">
                <FileText className="h-5 w-5 text-blue-400" />
                <span className="font-medium">{filename}</span>
              </div>
              <div className="text-right">
                <div className="text-sm font-medium">{(score * 100).toFixed(1)}% match</div>
                <div className="text-xs text-white/60 dark:text-gray-400">Score: {score.toFixed(4)}</div>
              </div>
            </div>
          ))}
        </div>
      ) : (
        <div className="text-center py-8 text-white/60 dark:text-gray-400">
          <FileText className="h-12 w-12 mx-auto mb-4" />
          No matching documents found.
        </div>
      )}
    </div>
  );
};

export default SearchResults;
