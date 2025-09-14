import { useState } from "react";
import { Folder, FileText, Loader2, CheckCircle } from "lucide-react";
import { buildIndex } from "@/services/api";

interface Props {
  onBuildSuccess: (msg: string) => void;
  onError: (err: string) => void;
  setIndexBuilt: (val: boolean) => void;
}

const DirectorySelector = ({ onBuildSuccess, onError, setIndexBuilt }: Props) => {
  const [directoryPath, setDirectoryPath] = useState("");
  const [isBuilding, setIsBuilding] = useState(false);
  const [buildStatus, setBuildStatus] = useState<string | null>(null);

  const handleBuildIndex = async () => {
    if (!directoryPath.trim()) {
      onError("Please enter a directory path");
      return;
    }

    setIsBuilding(true);
    setBuildStatus(null);

    try {
      const data = await buildIndex(directoryPath);
      setBuildStatus(data.message);
      onBuildSuccess(data.message);
      setIndexBuilt(true);
    } catch (err) {
      onError(`Failed to build index: ${err instanceof Error ? err.message : "Unknown error"}`);
    } finally {
      setIsBuilding(false);
    }
  };

  return (
    <div className="bg-white dark:bg-gray-800 rounded-lg shadow-sm border border-gray-200 dark:border-gray-700 p-6 mb-6">
      <div className="flex items-center space-x-2 mb-4">
        <Folder className="h-5 w-5 text-gray-600 dark:text-gray-400" />
        <h2 className="text-lg font-semibold text-gray-900 dark:text-white">
          Step 1: Select Document Directory
        </h2>
      </div>

      <div className="space-y-4">
        <input
          type="text"
          value={directoryPath}
          onChange={(e) => setDirectoryPath(e.target.value)}
          placeholder="Enter directory path"
          className="w-full px-3 py-2 border border-gray-300 dark:border-gray-600 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500 dark:bg-gray-700 dark:text-white"
        />

        <button
          onClick={handleBuildIndex}
          disabled={isBuilding || !directoryPath.trim()}
          className="flex items-center space-x-2 px-4 py-2 bg-blue-600 text-white rounded-md hover:bg-blue-700 disabled:bg-gray-400 disabled:cursor-not-allowed transition-colors"
        >
          {isBuilding ? (
            <>
              <Loader2 className="h-4 w-4 animate-spin" />
              <span>Building Index...</span>
            </>
          ) : (
            <>
              <FileText className="h-4 w-4" />
              <span>Build Index</span>
            </>
          )}
        </button>

        {buildStatus && (
          <div className="flex items-center space-x-2 text-green-600 dark:text-green-400">
            <CheckCircle className="h-4 w-4" />
            <span className="text-sm">{buildStatus}</span>
          </div>
        )}
      </div>
    </div>
  );
};

export default DirectorySelector;
