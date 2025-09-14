import { useState } from "react";
import { Search, Loader2 } from "lucide-react";
import { performSearch } from "@/services/api";
import { SearchResult } from "@/types/search";

interface Props {
  indexBuilt: boolean;
  onSearchSuccess: (results: SearchResult) => void;
  onError: (err: string) => void;
}

const SearchBox = ({ indexBuilt, onSearchSuccess, onError }: Props) => {
  const [searchQuery, setSearchQuery] = useState("");
  const [isSearching, setIsSearching] = useState(false);

  const handleSearch = async () => {
    if (!searchQuery.trim()) {
      onError("Please enter a search query");
      return;
    }
    if (!indexBuilt) {
      onError("Please build the index first");
      return;
    }

    setIsSearching(true);
    try {
      const data = await performSearch(searchQuery);
      onSearchSuccess(data);
    } catch (err) {
      onError(`Search failed: ${err instanceof Error ? err.message : "Unknown error"}`);
    } finally {
      setIsSearching(false);
    }
  };

  return (
    <div className="bg-white dark:bg-gray-800 rounded-lg shadow-sm border border-gray-200 dark:border-gray-700 p-6 mb-6">
      <div className="flex items-center space-x-2 mb-4">
        <Search className="h-5 w-5 text-gray-600 dark:text-gray-400" />
        <h2 className="text-lg font-semibold text-gray-900 dark:text-white">Step 2: Search Documents</h2>
      </div>

      <div className="space-y-4">
        <div className="flex space-x-3">
          <input
            type="text"
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.target.value)}
            onKeyPress={(e) => e.key === "Enter" && handleSearch()}
            placeholder="Enter your search query..."
            disabled={!indexBuilt}
            className="flex-1 px-3 py-2 border border-gray-300 dark:border-gray-600 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 dark:bg-gray-700 dark:text-white"
          />
          <button
            onClick={handleSearch}
            disabled={isSearching || !searchQuery.trim() || !indexBuilt}
            className="flex items-center space-x-2 px-4 py-2 bg-green-600 text-white rounded-md hover:bg-green-700 disabled:bg-gray-400 disabled:cursor-not-allowed transition-colors"
          >
            {isSearching ? <><Loader2 className="h-4 w-4 animate-spin" /><span>Searching...</span></> : <><Search className="h-4 w-4" /><span>Search</span></>}
          </button>
        </div>

        {!indexBuilt && <p className="text-sm text-gray-500 dark:text-gray-400">Please build the index first before searching.</p>}
      </div>
    </div>
  );
};

export default SearchBox;
