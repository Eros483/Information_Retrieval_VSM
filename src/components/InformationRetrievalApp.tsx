'use client';

import React, { useState } from "react";
import Header from "@/components/Header";
import DirectorySelector from "@/components/DirectorySelector";
import SearchBox from "@/components/SearchBox";
import SearchResults from "@/components/SearchResults";
import ErrorAlert from "@/components/ErrorAlert";
import InfoPanel from "@/components/InfoPanel";
import { SearchResult } from "@/types/search";

const InformationRetrievalApp = () => {
  const [searchResults, setSearchResults] = useState<SearchResult | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [indexBuilt, setIndexBuilt] = useState(false);

  const handleBuildSuccess = (msg: string) => {
    setError(null);
    console.log("Index built:", msg);
  };

  const handleSearchSuccess = (results: SearchResult) => {
    setError(null);
    setSearchResults(results);
  };

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-900">
      {/* Header */}
      <Header />

      <div className="max-w-4xl mx-auto px-6 py-8">
        {/* Directory Selection */}
        <DirectorySelector
          onBuildSuccess={handleBuildSuccess}
          onError={setError}
          setIndexBuilt={setIndexBuilt}
        />

        {/* Search */}
        <SearchBox
          indexBuilt={indexBuilt}
          onSearchSuccess={handleSearchSuccess}
          onError={setError}
        />

        {/* Error Display */}
        {error && <ErrorAlert message={error} />}

        {/* Search Results */}
        {searchResults && <SearchResults results={searchResults} />}

        {/* Info Panel */}
        <InfoPanel />
      </div>
    </div>
  );
};

export default InformationRetrievalApp;
