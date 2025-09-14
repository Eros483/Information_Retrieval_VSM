'use client';

import React from 'react';
import { AlertCircle } from 'lucide-react';

const ErrorAlert = ({ message }: { message: string }) => {
  return (
    <div className="bg-red-50 dark:bg-red-900/20 border border-red-200 dark:border-red-800 rounded-lg p-4 mb-6">
      <div className="flex items-center space-x-2">
        <AlertCircle className="h-5 w-5 text-red-500" />
        <p className="text-red-700 dark:text-red-300">{message}</p>
      </div>
    </div>
  );
};

export default ErrorAlert;
