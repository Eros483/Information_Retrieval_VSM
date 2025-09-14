'use client';

import React from 'react';

const InfoPanel = () => {
  return (
    <div className="mt-8 bg-blue-50 dark:bg-blue-900/20 border border-blue-200 dark:border-blue-800 rounded-lg p-4">
      <h4 className="font-semibold text-blue-900 dark:text-blue-300 mb-2">
        How to use:
      </h4>
      <ol className="text-sm text-blue-800 dark:text-blue-300 space-y-1">
        <li>1. Enter the path to your document directory (containing TXT, PDF files)</li>
        <li>2. Click "Build Index" to process your documents</li>
        <li>3. Enter your search query and click "Search"</li>
        <li>4. View ranked results with similarity scores</li>
      </ol>
    </div>
  );
};

export default InfoPanel;
