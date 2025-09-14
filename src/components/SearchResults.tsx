'use client';

import React from 'react';
import { FileText, Clock } from 'lucide-react';

interface SearchResult {
  query: string;
  results: [string, number][];
  elapsed_time: number;
}

const SearchResults = ({ results }: { results: SearchResult }) => {
  return (
    <div className="bg-white dark:bg-gray-800 rounded-lg shadow-sm border border-gray-200 dark:border-gray-700 p-6">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-gray-900 dark:text-white">
          Search Results
        </h3>
        <div className="flex items-center space-x-2 text-sm text-gray-500 dark:text-gray-400">
          <Clock className="h-4 w-4" />
          <span>{(results.elapsed_time * 1000).toFixed(2)}ms</span>
        </div>
      </div>
      
      <div className="mb-4">
        <p className="text-sm text-gray-600 dark:text-gray-400">
          Query: <span className="font-mono bg-gray-100 dark:bg-gray-700 px-2 py-1 rounded">
            {results.query}
          </span>
        </p>
      </div>

      {results.results.length > 0 ? (
        <div className="space-y-3">
          {results.results.map(([filename, score], index) => (
            <div
              key={index}
              className="flex items-center justify-between p-3 bg-gray-50 dark:bg-gray-700 rounded-lg border border-gray-200 dark:border-gray-600"
            >
              <div className="flex items-center space-x-3">
                <FileText className="h-5 w-5 text-blue-600" />
                <span className="font-medium text-gray-900 dark:text-white">
                  {filename}
                </span>
              </div>
              <div className="text-right">
                <div className="text-sm font-medium text-gray-900 dark:text-white">
                  {(score * 100).toFixed(1)}% match
                </div>
                <div className="text-xs text-gray-500 dark:text-gray-400">
                  Score: {score.toFixed(4)}
                </div>
              </div>
            </div>
          ))}
        </div>
      ) : (
        <div className="text-center py-8">
          <FileText className="h-12 w-12 text-gray-400 mx-auto mb-4" />
          <p className="text-gray-500 dark:text-gray-400">
            No matching documents found.
          </p>
        </div>
      )}
    </div>
  );
};

export default SearchResults;
