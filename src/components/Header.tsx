import { FileText } from "lucide-react";

const Header = () => (
  <div className="bg-white dark:bg-gray-800 border-b border-gray-200 dark:border-gray-700">
    <div className="max-w-4xl mx-auto px-6 py-4">
      <div className="flex items-center space-x-3">
        <FileText className="h-8 w-8 text-blue-600" />
        <div>
          <h1 className="text-2xl font-bold text-gray-900 dark:text-white">
            Information Retrieval VSM
          </h1>
          <p className="text-sm text-gray-600 dark:text-gray-400">
            Vector Space Model for Document Search
          </p>
        </div>
      </div>
    </div>
  </div>
);

export default Header;
